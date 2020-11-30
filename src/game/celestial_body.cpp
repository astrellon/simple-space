#include "celestial_body.hpp"

#include "../utils.hpp"

namespace space
{
    void CelestialBody::setTransformFromLocation()
    {
        const auto &location = celestialBodyDefinition.location;
        _transform.position = Utils::degreeVector(location.angle, location.distance);
    }
} // namespace space
