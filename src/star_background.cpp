#include "star_background.hpp"

#include <random>
#include <algorithm>

#include <SFML/OpenGL.hpp>

#include "engine.hpp"
#include "utils.hpp"

namespace space
{
    StarBackground::StarBackground(Engine &engine, const StarBackgroundOptions &options) :
        _engine(engine), _options(options)
    {
        for (auto i = 0; i < options.numLayers; i++)
        {
            _layers.emplace_back(std::make_unique<StarBackgroundLayer>(*this, 0.9f - (7 - i) * 0.1f));
        }
    }

    void StarBackground::update(sf::Time dt)
    {
        for (auto &layer : _layers)
        {
            layer->update(dt);
        }
    }

    void StarBackground::draw(sf::RenderTarget &target)
    {
        //target.clear(sf::Color(20, 24, 46));
        target.clear(_options.backgroundColour);
        shader()->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        for (auto &layer : _layers)
        {
            layer->draw(target);
        }

        glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    }

    // StarBackgroundLayer
    StarBackgroundLayer::StarBackgroundLayer(StarBackground &parent, float distanceScale) :
          _parent(parent), _distanceScale(distanceScale), _camera(parent.engine().sceneRender().camera(), distanceScale)
    {

    }

    void StarBackgroundLayer::update(sf::Time dt)
    {
        _camera.update(dt);
        auto center = _camera.view().getCenter();
        auto size = _camera.view().getSize() * 0.6f;
        auto lowerX = (int)floor((center.x - size.x) / _parent.area());
        auto upperX = (int)ceil((center.x + size.x) / _parent.area());
        auto lowerY = (int)floor((center.y - size.y) / _parent.area());
        auto upperY = (int)ceil((center.y + size.y) / _parent.area());
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
    void StarBackgroundLayer::draw(sf::RenderTarget &target)
    {
        auto shader = _parent.shader();
        sf::RenderStates states;
        states.shader = shader;

        target.setView(_camera.view());

        auto pointSize = std::max(1, static_cast<int>(std::roundf(_distanceScale * _camera.camera().scale())));

        shader->setUniform("distanceScale", _distanceScale);
        shader->setUniform("pointSize", pointSize);

        for (auto &chunk : _chunks)
        {
            if (!chunk->isActive())
            {
                continue;
            }

            chunk->draw(target, states);
        }
    }

    StarBackgroundChunk *StarBackgroundLayer::getChunk(sf::Vector2i pos)
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

        auto newChunk = std::make_unique<StarBackgroundChunk>(*this);
        newChunk->position(pos);

        auto result = newChunk.get();
        _chunks.emplace_back(std::move(newChunk));

        return result;
    }

    // Chunks
    StarBackgroundChunk::StarBackgroundChunk(StarBackgroundLayer &parent) :
        _parent(parent), _vertices(sf::Points, parent.numParticles()), _active(false)
    {

    }

    void StarBackgroundChunk::position(sf::Vector2i position)
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
        std::uniform_real_distribution<float> colourRange(distanceScale * 100 + 20, distanceScale * 100 + 120);

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

    void StarBackgroundChunk::draw(sf::RenderTarget &target, sf::RenderStates &states)
    {
        target.draw(_vertices, states);
    }
} // namespace space
