#pragma once

#include <SFML/Graphics.hpp>
#include "json.hpp"

using nlohmann::json;

namespace space
{
    class BaseDefinition;
    class ShipDefinition;
    class CharacterDefinition;
    class CelestialBodyDefinition;
    class OrbitPointCelestialDefinition;
    class PlanetDefinition;
    class StarSystemDefinition;
    class PlanetSurfaceDefinition;
    class CelestialBodyLocation;
    class StarBackgroundOptions;
    class Dialogue;

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

    json toJson(const PlanetSurfaceDefinition &input);
    std::unique_ptr<PlanetSurfaceDefinition> fromJsonPlanetSurfaceDefinition(const json &j);

    json toJson(const Dialogue &input);
    std::unique_ptr<Dialogue> fromJsonDialogue(const json &j);

    json toJson(const CelestialBodyLocation &input);
    CelestialBodyLocation fromJsonCelestialBodyLocation(const json &j);

    json toJson(const StarBackgroundOptions &input);
    StarBackgroundOptions fromJsonStarBackgroundOptions(const json &j);

} // namespace space