#include "star_background_2.hpp"

#include <random>
#include <algorithm>

#include "utils.hpp"

namespace space
{
    StarBackground2::StarBackground2(Engine &engine, float area, int numParticles, float distanceScale, sf::Vector2i position) : _vertices(sf::Points, numParticles), _area(area), _distanceScale(distanceScale), _camera(engine)
    {
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

    void StarBackground2::draw(sf::RenderTarget &target)
    {
        target.draw(_vertices);
    }

    void StarBackground2::setCameraCenter(sf::Vector2f center)
    {
        _camera.center(center * _distanceScale);
    }

} // namespace space
