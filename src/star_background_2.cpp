#include "star_background_2.hpp"

#include <random>
#include <algorithm>

#include "engine.hpp"
#include "utils.hpp"

namespace space
{
    StarBackground2::StarBackground2(Engine &engine, sf::Shader *shader, float area, int numParticles, float distanceScale) :
        _engine(engine), _area(area), _shader(shader), _distanceScale(distanceScale), _camera(engine), _numParticles(numParticles)
    {
    }

    void StarBackground2::update(sf::Time dt)
    {
        auto center = _camera.view().getCenter();
        auto size = _camera.view().getSize() * 0.6f;
        auto lowerX = (int)floor((center.x - size.x) / _area);
        auto upperX = (int)ceil((center.x + size.x) / _area);
        auto lowerY = (int)floor((center.y - size.y) / _area);
        auto upperY = (int)ceil((center.y + size.y) / _area);
        sf::IntRect bounds(lowerX, lowerY, upperX - lowerX, upperY - lowerY);

        for (auto &chunk : _chunks)
        {
            auto inBounds = bounds.contains(chunk->position());
            chunk->active(inBounds);
        }

        for (auto x = lowerX; x <= upperX; x++)
        {
            for (auto y = lowerY; y <= upperY; y++)
            {
                sf::Vector2i pos(x, y);
                auto chunk = getChunk(pos);
                if (chunk->position() != pos)
                {
                    chunk->position(pos);
                    chunk->active(true);
                }
            }
        }
    }
    void StarBackground2::draw(sf::RenderTarget &target)
    {
        sf::RenderStates states;
        states.shader = _shader;

        auto pointSize = std::max(1, static_cast<int>(std::roundf(_distanceScale * _camera.scale())));

        _shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());
        _shader->setUniform("distanceScale", _distanceScale);
        _shader->setUniform("pointSize", pointSize);

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        for (auto &chunk : _chunks)
        {
            if (!chunk->isActive())
            {
                continue;
            }

            chunk->draw(target, states);
        }

        glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    }

    void StarBackground2::onResize(sf::Vector2f size)
    {
        _camera.size(size);
    }

    void StarBackground2::cameraCenter(sf::Vector2f center)
    {
        _camera.center(center * _distanceScale);
    }

    StarBackgroundChunk2 *StarBackground2::getChunk(sf::Vector2i pos)
    {
        for (auto &it : _chunks)
        {
            if (it->position() == pos)
            {
                return it.get();
            }
        }

        for (auto &it : _chunks)
        {
            if (!it->isActive())
            {
                return it.get();
            }
        }

        auto newChunk = std::make_unique<StarBackgroundChunk2>(*this);
        newChunk->position(pos);

        auto result = newChunk.get();
        _chunks.emplace_back(std::move(newChunk));

        return result;
    }

    // Chunks
    StarBackgroundChunk2::StarBackgroundChunk2(StarBackground2 &parent) :
        _parent(parent), _vertices(sf::Points, parent.numParticles()), _active(false)
    {

    }

    void StarBackgroundChunk2::position(sf::Vector2i position)
    {
        _position = position;

        auto area = _parent.area();
        auto distanceScale = _parent.distanceScale();
        auto numParticles = _parent.numParticles();
        auto seed = (position.x + (position.y << 16)) + distanceScale * 255;

        auto rand = Utils::randWithSeed(seed);

        auto positionOffset = sf::Vector2f(position) * area;

        std::uniform_real_distribution<float> bigStarRange(0, 100);
        std::uniform_real_distribution<float> posRange(0, area);
        std::uniform_real_distribution<float> colourRange(127 - distanceScale * 100, 255 - distanceScale * 100);

        for (auto i = 0; i < numParticles; i++)
        {
            auto &vertex = _vertices[i];
            vertex.position = sf::Vector2f(posRange(rand), posRange(rand)) + positionOffset;

            sf::Color colour;
            colour.r = static_cast<int>(colourRange(rand));
            colour.g = static_cast<int>(colourRange(rand));
            colour.b = static_cast<int>(colourRange(rand));
            vertex.color = colour;
        }
    }

    void StarBackgroundChunk2::draw(sf::RenderTarget &target, sf::RenderStates &states)
    {
        target.draw(_vertices, states);
    }
} // namespace space
