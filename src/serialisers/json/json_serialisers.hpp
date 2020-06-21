#pragma once

#include <SFML/Graphics.hpp>

#include "../../definitions/base_definition.hpp"
#include "../../definitions/ship_definition.hpp"
#include "../../definitions/celestial_body_definition.hpp"
#include "../../definitions/planet_definition.hpp"
#include "json.hpp"

using nlohmann::json;

namespace space
{
    json toJson(const BaseDefinition &input);
    std::unique_ptr<BaseDefinition> fromJsonBaseDefinition(const json &j);

    json toJson(const ShipDefinition &input);
    std::unique_ptr<ShipDefinition> fromJsonShipDefinition(const json &j);

    std::unique_ptr<CelestialBodyDefinition> fromJsonCelestialBodyDefinition(const json &j);

    json toJson(const OrbitPointCelestialDefinition &input);
    std::unique_ptr<OrbitPointCelestialDefinition> fromJsonOrbitPointCelestialDefinition(const json &j);

    json toJson(const PlanetDefinition &input);
    std::unique_ptr<PlanetDefinition> fromJsonPlanetDefinition(const json &j);

    std::string toJson(const sf::Color &input);

    json toJson(const CelestialBodyLocation &input);
    CelestialBodyLocation fromJsonCelestialBodyLocation(const json &j);

} // namespace space