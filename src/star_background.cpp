#include "star_background.hpp"

#include <random>
#include <sstream>
#include <algorithm>

#include "engine.hpp"
#include "utils.hpp"

namespace space
{
    StarBackground::StarBackground(Engine &engine, int numParticlesPerChunk, float chunkArea, float distanceScale)
        : Particles(engine, 0, GL_STATIC_DRAW),
        _shader(nullptr), _numParticlesPerChunk(numParticlesPerChunk),
        _distanceScale(distanceScale), _chunkArea(chunkArea), _numChunks(0), _nextAvailableIndex(0)
    {

    }

    void StarBackground::onResize(sf::Vector2f viewport)
    {
        _inited = false;
        _viewport = viewport;

        auto dx = viewport.x / _chunkArea;
        auto dy = viewport.y / _chunkArea;

        _chunkDims.x = std::ceil(dx) + 2;
        _chunkDims.y = std::ceil(dy) + 2;

        _nextAvailableIndex = 0;

        _chunks.clear();
        _availableIndicies.clear();

        _numChunks = _chunkDims.x * _chunkDims.y;
        _numParticles = _numChunks * _numParticlesPerChunk;
    }

    void StarBackground::onInit()
    {
        if (_shader == nullptr)
        {
            if (!_engine.resourceManager().shader("stars", &_shader))
            {
                std::cout << "Unable to find shader for star background" << std::endl;
                return;
            }
        }
    }

    void StarBackground::onUpdate(sf::Time dt)
    {
        auto &view = _engine.camera().view();
        auto center = view.getCenter() * _distanceScale;
        auto size = view.getSize() * 0.6f;

        auto min = Utils::floor((center - size) / _chunkArea);
        auto max = Utils::ceil((center + size) / _chunkArea);

        auto needsSync = false;

        std::vector<std::pair<std::pair<int, int>, int>> availableOffsets;

        for (auto &it : _chunks)
        {
            auto &pos = it.first;
            if (pos.first < min.x || pos.first > max.x ||
                pos.second < min.y || pos.second > max.y)
            {
                availableOffsets.emplace_back(std::pair<std::pair<int, int>, int>(pos, it.second.indexOffset()));
            }
        }

        for (auto &it : availableOffsets)
        {
            _chunks.erase(it.first);
            _availableIndicies.push_back(it.second);
        }

        for (auto y = min.y; y <= max.y; y++)
        for (auto x = min.x; x <= max.x; x++)
        {
            std::pair<int, int> pos(x, y);

            if (_chunks.find(pos) != _chunks.end())
            {
                continue;
            }

            int index;
            if (_availableIndicies.size() > 0)
            {
                index = *_availableIndicies.rbegin();
                _availableIndicies.pop_back();
            }
            else
            {
                if (_nextAvailableIndex >= _numParticles)
                {
                    std::cout << "Ran out of available indicies" << std::endl;
                    break;
                }

                index = _nextAvailableIndex;
                _nextAvailableIndex += _numParticlesPerChunk;
            }

            needsSync = true;

            auto chunk = _chunks.emplace(pos, index);
            chunk.first->second.updatePosition(*this, sf::Vector2i(x, y));
        }

        if (needsSync)
        {
            syncPositionsColours();
        }
    }

    bool StarBackground::onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (_shader == nullptr)
        {
            return false;
        }

        auto combinedTransform = _engine.camera().view().getTransform() * parentTransform;

        sf::Glsl::Mat4 mat4(combinedTransform.getMatrix());

        mat4.array[12] *= _distanceScale;
        mat4.array[13] *= _distanceScale;

        _shader->setUniform("transform", mat4);

        return true;
    }

    void StarBackground::onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
    }

    void StarBackground::bindShader(const sf::Transform &parentTransform)
    {
        sf::Shader::bind(_shader);
        _shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());
    }

    void StarBackground::unbindShader()
    {
        sf::Shader::bind(nullptr);
    }

    void StarBackground::drawDebug(sf::RenderTarget &target)
    {
        auto font = _engine.resourceManager().defaultFont();
        // std::cout << "Chunks: ";
        for (auto &chunk : _chunks)
        {
            std::stringstream ss;
            ss << chunk.first.first << ", " << chunk.first.second;
            // std::cout << "[" << chunk.first.first << ", " << chunk.first.second << "], ";
            sf::Text text(ss.str(), *font);
            text.setCharacterSize(24);

            sf::RenderStates renderState;
            renderState.transform = sf::Transform::Identity;
            renderState.transform.translate(chunk.first.first * _chunkArea, chunk.first.second * _chunkArea);

            target.draw(text, renderState);
        }
        // std::cout << std::endl;
    }

    // Chunks
    StarBackgroundChunk::StarBackgroundChunk(int indexOffset)
        : _indexOffset(indexOffset)
    {

    }

    void StarBackgroundChunk::updatePosition(StarBackground &parent, sf::Vector2i position)
    {
        auto area = parent._chunkArea;
        auto distanceScale = parent._distanceScale;
        sf::Vector2f offset(position.x * area, position.y * area);
        auto rand = Utils::randWithSeed((position.x + (position.y << 16)) / distanceScale + distanceScale * 255);

        std::uniform_real_distribution<float> bigStarRange(0, 100);
        std::uniform_real_distribution<float> xRange(offset.x, area + offset.x);
        std::uniform_real_distribution<float> yRange(offset.y, area + offset.y);
        std::uniform_real_distribution<float> colourRange(127 * distanceScale * distanceScale, 235 * distanceScale * distanceScale);

        auto endIndex = _indexOffset + parent._numParticlesPerChunk;
        for (auto i = _indexOffset; i < endIndex; i++)
        {
            auto isBigStar = false;
            if (i < endIndex - 5)
            {
                isBigStar = bigStarRange(rand) < 5;
            }
            auto &position = parent._positions[i];
            auto &colour = parent._colours[i];
            position.x = xRange(rand);
            position.y = yRange(rand);

            colour.r = static_cast<int>(colourRange(rand));
            colour.g = static_cast<int>(colourRange(rand));
            colour.b = static_cast<int>(colourRange(rand));

            if (isBigStar)
            {
                sf::Color dimmedColour(colour.r * 0.6, colour.g * 0.6, colour.b * 0.6);
                colour.r = std::min(255, static_cast<int>(colour.r * 1.4));
                colour.g = std::min(255, static_cast<int>(colour.g * 1.4));
                colour.b = std::min(255, static_cast<int>(colour.b * 1.4));

                parent._positions[i + 1].x = position.x - 1;
                parent._positions[i + 1].y = position.y;
                parent._positions[i + 2].x = position.x + 1;
                parent._positions[i + 2].y = position.y;
                parent._positions[i + 3].x = position.x;
                parent._positions[i + 3].y = position.y - 1;
                parent._positions[i + 4].x = position.x;
                parent._positions[i + 4].y = position.y + 1;

                Utils::setColour(parent._colours[i + 1], dimmedColour);
                Utils::setColour(parent._colours[i + 2], dimmedColour);
                Utils::setColour(parent._colours[i + 3], dimmedColour);
                Utils::setColour(parent._colours[i + 4], dimmedColour);

                i += 5;
            }
        }
    }
} // namespace space
