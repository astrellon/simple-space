#pragma once

#include <SFML/Graphics.hpp>

#include "../../definitions/base_definition.hpp"
#include "../../definitions/ship_definition.hpp"
#include "../../definitions/character_definition.hpp"
#include "../../definitions/celestial_body_definition.hpp"
#include "../../definitions/planet_definition.hpp"
#include "../../definitions/star_system_definition.hpp"
#include "json.hpp"

using nlohmann::json;

namespace space
{
    json toJson(const BaseDefinition &input);
    std::unique_ptr<BaseDefinition> fromJsonBaseDefinition(const json &j);

    json toJson(const ShipDefinition &input);
    std::unique_ptr<ShipDefinition> fromJsonShipDefinition(const json &j);

    json toJson(const CharacterDefinition &input);
    std::unique_ptr<CharacterDefinition> fromJsonCharacterDefinition(const json &j);

    std::unique_ptr<CelestialBodyDefinition> fromJsonCelestialBodyDefinition(const json &j);

    json toJson(const OrbitPointCelestialDefinition &input);
    std::unique_ptr<OrbitPointCelestialDefinition> fromJsonOrbitPointCelestialDefinition(const json &j);

    json toJson(const PlanetDefinition &input);
    std::unique_ptr<PlanetDefinition> fromJsonPlanetDefinition(const json &j);

    json toJson(const StarSystemDefinition &input);
    std::unique_ptr<StarSystemDefinition> fromJsonStarSystemDefinition(const json &j);

    json toJson(const CelestialBodyLocation &input);
    CelestialBodyLocation fromJsonCelestialBodyLocation(const json &j);

} // namespace space