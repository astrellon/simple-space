#include "celestial_body.hpp"

namespace space
{
    void CelestialBody::setTransformFromLocation()
    {
        const auto &location = celestialBodyDefinition.location;
        auto x = std::cos(location.angle) * location.distance;
        auto y = std::sin(location.angle) * location.distance;
        _transform.position = sf::Vector2f(x, y);
    }
} // namespace space
