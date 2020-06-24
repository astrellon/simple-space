#include "star_background_2.hpp"

#include <random>
#include <algorithm>

#include "utils.hpp"

namespace space
{
    StarBackground2::StarBackground2(Engine &engine, float area, int numParticles, float distanceScale) : _area(area), _distanceScale(distanceScale), _camera(engine), _numParticles(numParticles)
    {
    }

    void StarBackground2::draw(sf::RenderTarget &target)
    {
        for (auto &chunk : _chunks)
        {
            chunk->draw(target);
        }
    }

    void StarBackground2::onResize(sf::Vector2f size)
    {
        _camera.size(size);
    }

    void StarBackground2::setCameraCenter(sf::Vector2f center)
    {
        _camera.center(center * _distanceScale);
    }

    // Chunks
    StarBackgroundChunk2::StarBackgroundChunk2(StarBackground2 &parent) :
        _parent(parent), _vertices(sf::Points, parent.numParticles())
    {

    }

    void StarBackgroundChunk2::setPosition(sf::Vector2i position)
    {
        auto area = _parent.area();
        auto distanceScale = _parent.area();
        auto numParticles = _parent.numParticles();

        auto rand = Utils::randWithSeed((position.x + (position.y << 16)) / distanceScale + distanceScale * 255);

        auto positionOffset = sf::Vector2f(position) * area;

        std::uniform_real_distribution<float> bigStarRange(0, 100);
        std::uniform_real_distribution<float> posRange(-area, area);
        std::uniform_real_distribution<float> colourRange(127, 255);

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

    void StarBackgroundChunk2::draw(sf::RenderTarget &target)
    {
        target.draw(_vertices);
    }
} // namespace space
