#include "celestial_body.hpp"

#include "../utils.hpp"

namespace space
{
    void CelestialBody::setTransformFromLocation()
    {
        const auto &location = celestialBodyDefinition.location;
        auto x = std::cos(Utils::degreesToRadians(location.angle)) * location.distance;
        auto y = std::sin(Utils::degreesToRadians(location.angle)) * location.distance;
        _transform.position = sf::Vector2f(x, y);
    }
} // namespace space
