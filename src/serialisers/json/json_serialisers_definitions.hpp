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
    class PlaceableItemDefinition;
    class AnimatedTexture;
    class AnimationSequence;
    class SpacePortalDefinition;
    class ShaderDefinition;
    class GrassEffectDefinition;
    class CompendiumDefinition;

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

    json toJson(const PlaceableItemDefinition &input);
    std::unique_ptr<PlaceableItemDefinition> fromJsonPlaceableItemDefinition(const json &j);

    json toJson(const SpacePortalDefinition &input);
    std::unique_ptr<SpacePortalDefinition> fromJsonSpacePortalDefinition(const json &j);

    json toJson(const AnimatedTexture &input);
    std::unique_ptr<AnimatedTexture> fromJsonAnimatedTexture(const json &j);

    json toJson(const AnimationSequence &input);
    AnimationSequence fromJsonAnimationSequence(const json &j);

    json toJson(const ShaderDefinition &input);
    std::unique_ptr<ShaderDefinition> fromJsonShaderDefinition(const json &j);

    json toJson(const GrassEffectDefinition &input);
    std::unique_ptr<GrassEffectDefinition> fromJsonGrassEffectDefinition(const json &j);

    json toJson(const CompendiumDefinition &input);
    std::unique_ptr<CompendiumDefinition> fromJsonCompendiumDefinition(const json &j);

    json toJson(const CelestialBodyLocation &input);
    CelestialBodyLocation fromJsonCelestialBodyLocation(const json &j);

    json toJson(const StarBackgroundOptions &input);
    StarBackgroundOptions fromJsonStarBackgroundOptions(const json &j);

} // namespace space