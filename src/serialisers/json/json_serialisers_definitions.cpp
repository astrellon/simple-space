#include "json_serialisers_definitions.hpp"

#include <iostream>
#include <stdexcept>

#include "json_common.hpp"
#include "json_serialisers_game.hpp"

#include "../../utils.hpp"

#include "../../definitions/base_definition.hpp"
#include "../../definitions/ship_definition.hpp"
#include "../../definitions/character_definition.hpp"
#include "../../definitions/celestial_body_definition.hpp"
#include "../../definitions/planet_definition.hpp"
#include "../../definitions/star_system_definition.hpp"
#include "../../definitions/planet_surface_definition.hpp"
#include "../../definitions/star_background_options.hpp"
#include "../../definitions/dialogue.hpp"
#include "../../definitions/item_definition.hpp"
#include "../../definitions/placeable_item_definition.hpp"
#include "../../definitions/animated_texture.hpp"
#include "../../definitions/space_portal_definition.hpp"
#include "../../definitions/shader_definition.hpp"
#include "../../definitions/grass_effect_definition.hpp"
#include "../../definitions/compendium_definition.hpp"
#include "../../definitions/dialogue_audio.hpp"
#include "../../definitions/cursor.hpp"

using nlohmann::json;

namespace space
{
    json toJson(const BaseDefinition &input)
    {
        json j;
        auto type = input.type();
        if (type == ShipDefinition::DefinitionType())
            j = toJson(dynamic_cast<const ShipDefinition &>(input));

        else if (type == CharacterDefinition::DefinitionType())
            j = toJson(dynamic_cast<const CharacterDefinition &>(input));

        else if (type == PlanetDefinition::DefinitionType())
            j = toJson(dynamic_cast<const PlanetDefinition &>(input));

        else if (type == OrbitPointCelestialDefinition::DefinitionType())
            j = toJson(dynamic_cast<const OrbitPointCelestialDefinition &>(input));

        else if (type == StarSystemDefinition::DefinitionType())
            j = toJson(dynamic_cast<const StarSystemDefinition &>(input));

        else if (type == PlanetSurfaceDefinition::DefinitionType())
            j = toJson(dynamic_cast<const PlanetSurfaceDefinition &>(input));

        else if (type == Dialogue::DefinitionType())
            j = toJson(dynamic_cast<const Dialogue &>(input));

        else if (type == PlaceableItemDefinition::DefinitionType())
            j = toJson(dynamic_cast<const PlaceableItemDefinition &>(input));

        else if (type == AnimatedTexture::DefinitionType())
            j = toJson(dynamic_cast<const AnimatedTexture &>(input));

        else if (type == SpacePortalDefinition::DefinitionType())
            j = toJson(dynamic_cast<const SpacePortalDefinition &>(input));

        else if (type == ShaderDefinition::DefinitionType())
            j = toJson(dynamic_cast<const ShaderDefinition &>(input));

        else if (type == GrassEffectDefinition::DefinitionType())
            j = toJson(dynamic_cast<const GrassEffectDefinition &>(input));

        else if (type == CompendiumDefinition::DefinitionType())
            j = toJson(dynamic_cast<const CompendiumDefinition &>(input));

        else if (type == Cursor::DefinitionType())
            j = toJson(dynamic_cast<const Cursor &>(input));

        else if (type == DialogueAudio::DefinitionType())
            j = toJson(dynamic_cast<const Cursor &>(input));

        else
            std::cout << "Error!" << std::endl;

        j["type"] = type;
        return j;
    }

    std::unique_ptr<BaseDefinition> fromJsonBaseDefinition(const json &j)
    {
        auto type = j.at("type").get<std::string>();

        if (type == ShipDefinition::DefinitionType())
            return fromJsonShipDefinition(j);

        if (type == CharacterDefinition::DefinitionType())
            return fromJsonCharacterDefinition(j);

        if (type == PlanetDefinition::DefinitionType() ||
            type == OrbitPointCelestialDefinition::DefinitionType())
            return fromJsonCelestialBodyDefinition(j);

        if (type == StarSystemDefinition::DefinitionType())
            return fromJsonStarSystemDefinition(j);

        if (type == PlanetSurfaceDefinition::DefinitionType())
            return fromJsonPlanetSurfaceDefinition(j);

        if (type == Dialogue::DefinitionType())
            return fromJsonDialogue(j);

        if (type == PlaceableItemDefinition::DefinitionType())
            return fromJsonPlaceableItemDefinition(j);

        if (type == AnimatedTexture::DefinitionType())
            return fromJsonAnimatedTexture(j);

        if (type == SpacePortalDefinition::DefinitionType())
            return fromJsonSpacePortalDefinition(j);

        if (type == ShaderDefinition::DefinitionType())
            return fromJsonShaderDefinition(j);

        if (type == GrassEffectDefinition::DefinitionType())
            return fromJsonGrassEffectDefinition(j);

        if (type == CompendiumDefinition::DefinitionType())
            return fromJsonCompendiumDefinition(j);

        if (type == DialogueAudio::DefinitionType())
            return fromJsonDialogueAudio(j);

        if (type == Cursor::DefinitionType())
            return fromJsonCursor(j);

        throw std::runtime_error("Oh no");
    }

    json toJson(const ShipDefinition &input)
    {
        return json {
            {"id", input.id},
            {"texturePath", input.texturePath},
            {"interiorTexturePath", input.interiorTexturePath},
            {"interiorTextureOffset", toJson(input.interiorTextureOffset)},
            {"engineGlowTexturePath", input.engineGlowTexturePath},
            {"name", input.name},
            {"maxRotation", input.maxRotation},
            {"maxSpeed", input.maxSpeed},
            {"turnRate", input.turnRate},
            {"acceleration", input.acceleration},
            {"interiorPolygon", toJsonArray(input.interiorPolygon)},
            {"extraInteriorPolygons", toJsonArray(input.extraInteriorPolygons)},
            {"enginePlacements", toJsonArray(input.enginePlacements)}
        };
    }

    std::unique_ptr<ShipDefinition> fromJsonShipDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<ShipDefinition>(id);
        j.at("texturePath").get_to(result->texturePath);
        j.at("interiorTexturePath").get_to(result->interiorTexturePath);

        Utils::json_try_get(j, "interiorTextureOffset", result->interiorTextureOffset);

        auto interiorPolygon = j.find("interiorPolygon");
        if (interiorPolygon != j.end())
        {
            for (auto &child : *interiorPolygon)
            {
                auto pos = fromJsonVector2f(child);
                result->interiorPolygon.push_back(pos);
            }
        }

        auto extraInteriorPolygons = j.find("extraInteriorPolygons");
        if (extraInteriorPolygons != j.end())
        {
            for (auto &polygonJson : *extraInteriorPolygons)
            {
                Polygon polygon;
                for (auto &posJson : polygonJson)
                {
                    polygon.push_back(fromJsonVector2f(posJson));
                }
                result->extraInteriorPolygons.push_back(polygon);
            }
        }

        auto enginePlacements = j.find("enginePlacements");
        if (enginePlacements != j.end())
        {
            for (auto &child : *enginePlacements)
            {
                sf::Vector2f pos(child[0].get<float>(), child[1].get<float>());
                result->enginePlacements.push_back(pos);
            }
        }

        Utils::json_try_get(j, "engineGlowTexturePath", result->engineGlowTexturePath);
        j.at("name").get_to(result->name);
        j.at("maxRotation").get_to(result->maxRotation);
        j.at("maxSpeed").get_to(result->maxSpeed);
        j.at("turnRate").get_to(result->turnRate);
        j.at("acceleration").get_to(result->acceleration);

        return result;
    }

    json toJson(const CharacterDefinition &input)
    {
        return json {
            {"id", input.id},
            {"animatedTextureId", input.animatedTextureId},
            {"name", input.name},
            {"spriteSize", input.spriteSize},
            {"speed", input.speed},
            {"compendiumId", input.compendiumId},
            {"dialogueAudioId", input.dialogueAudioId}
        };
    }

    std::unique_ptr<CharacterDefinition> fromJsonCharacterDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto input = std::make_unique<CharacterDefinition>(id);
        j.at("animatedTextureId").get_to(input->animatedTextureId);
        j.at("name").get_to(input->name);
        j.at("spriteSize").get_to(input->spriteSize);
        j.at("speed").get_to(input->speed);

        // Optional
        Utils::json_try_get(j, "compendiumId", input->compendiumId);
        Utils::json_try_get(j, "dialogueAudioId", input->dialogueAudioId);

        return input;
    }

    std::unique_ptr<CelestialBodyDefinition> fromJsonCelestialBodyDefinition(const json &j)
    {
        auto type = j.at("type").get<std::string>();

        if (type == PlanetDefinition::DefinitionType())
        {
            return fromJsonPlanetDefinition(j);
        }
        else if (type == OrbitPointCelestialDefinition::DefinitionType())
        {
            return fromJsonOrbitPointCelestialDefinition(j);
        }

        throw std::runtime_error("Oh no");
    }

    json toJson(const OrbitPointCelestialDefinition &input)
    {
        return json {
            {"id", input.id},
            {"location", toJson(input.location)},
            {"children", toJsonArray(input.children)}
        };
    }

    std::unique_ptr<OrbitPointCelestialDefinition> fromJsonOrbitPointCelestialDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<OrbitPointCelestialDefinition>(id);
        result->location = fromJsonCelestialBodyLocation(j.at("location"));

        auto childrenJson = j.find("children");
        if (childrenJson != j.end())
        {
            for (auto &childJson : *childrenJson)
            {
                result->children.emplace_back(std::move(fromJsonCelestialBodyDefinition(childJson)));
            }
        }

        return result;
    }

    json toJson(const PlanetDefinition &input)
    {
        return json
        {
            {"id", input.id},
            {"compendiumId", input.compendiumId},
            {"location", toJson(input.location)},
            {"children", toJsonArray(input.children)},
            {"texturePath", input.texturePath},
            {"name", input.name},
            {"rotationRate", input.rotationRate},
            {"size", input.size},
            {"scale", input.scale},
            {"glowColour", Utils::toHexString(input.glowColour)}
        };
    }

    std::unique_ptr<PlanetDefinition> fromJsonPlanetDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<PlanetDefinition>(id);
        result->location = fromJsonCelestialBodyLocation(j.at("location"));

        auto childrenJson = j.find("children");
        if (childrenJson != j.end())
        {
            for (auto &childJson : *childrenJson)
            {
                result->children.emplace_back(std::move(fromJsonCelestialBodyDefinition(childJson)));
            }
        }

        j.at("texturePath").get_to(result->texturePath);
        j.at("name").get_to(result->name);
        j.at("rotationRate").get_to(result->rotationRate);
        j.at("size").get_to(result->size);
        j.at("scale").get_to(result->scale);

        Utils::json_try_get(j, "oscillateNoise", result->oscillateNoise);
        Utils::json_try_get(j, "compendiumId", result->compendiumId);

        auto planetSurfaceIds = j.find("planetSurfaceIds");
        if (planetSurfaceIds != j.end())
        {
            for (auto &idJson : *planetSurfaceIds)
            {
                result->planetSurfaceIds.push_back(idJson.get<std::string>());
            }
        }

        result->glowColour = Utils::fromHexString(j.at("glowColour").get<std::string>());

        return result;
    }

    json toJson(const StarSystemDefinition &input)
    {
        return json {
            {"id", input.id},
            {"name", input.name},
            {"rootBody", toJson(*input.rootBody.get())},
            {"starBackgroundOptions", toJson(input.starBackgroundOptions)}
        };
    }

    std::unique_ptr<StarSystemDefinition> fromJsonStarSystemDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<StarSystemDefinition>(id);
        j.at("name").get_to(result->name);

        result->rootBody = fromJsonCelestialBodyDefinition(j.at("rootBody"));
        result->starBackgroundOptions = fromJsonStarBackgroundOptions(j.at("starBackgroundOptions"));

        return result;
    }

    json toJson(const PlanetSurfaceDefinition &input)
    {
        return json {
            {"id", input.id},
            {"name", input.name},
            {"tmxMapPath", input.tmxMapPath}
        };
    }

    std::unique_ptr<PlanetSurfaceDefinition> fromJsonPlanetSurfaceDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<PlanetSurfaceDefinition>(id);
        j.at("name").get_to(result->name);
        j.at("tmxMapPath").get_to(result->tmxMapPath);

        auto walkableAreaInstancesJson = j.find("walkableAreaInstances");
        if (walkableAreaInstancesJson != j.end())
        {
            // fromJsonWalkableAreaInstances(*walkableAreaInstancesJson, result->walkableAreaInstances);
        }

        return result;
    }

    json toJson(const Dialogue &input)
    {
        return json {
            {"id", input.id},
            {"text", toJsonArray(input.text)}
        };
    }

    std::unique_ptr<Dialogue> fromJsonDialogue(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        std::vector<std::string> text;

        auto textJson = j.find("text");
        if (textJson != j.end())
        {
            for (auto &lineJson : *textJson)
            {
                text.push_back(lineJson.get<std::string>());
            }
        }

        return std::make_unique<Dialogue>(id, text);
    }

    json toJson(const PlaceableItemDefinition &input)
    {
        auto result = json {
            {"id", input.id},
            {"texturePath", input.texturePath},
            {"canPickup", input.canPickup},
            {"canUse", input.canUse}
        };

        if (input.textureOffset != sf::Vector2f())
        {
            result["textureOffset"] = toJson(input.textureOffset);
        }

        if (input.physicsShape.type() != PhysicsShape::Unknown)
        {
            result["physicsShape"] = toJson(input.physicsShape);
        }

        return result;
    }

    std::unique_ptr<PlaceableItemDefinition> fromJsonPlaceableItemDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<PlaceableItemDefinition>(id);
        j.at("name").get_to(result->name);
        j.at("texturePath").get_to(result->texturePath);
        Utils::json_try_get(j, "canPickup", result->canPickup);
        Utils::json_try_get(j, "canUse", result->canUse);
        Utils::json_try_get(j, "textureOffset", result->textureOffset);

        auto physicsShapeFind = j.find("physicsShape");
        if (physicsShapeFind != j.end())
        {
            result->physicsShape = fromJsonPhysicsShape(*physicsShapeFind);
        }

        std::string drawLayerString;
        if (Utils::json_try_get(j, "drawLayer", drawLayerString))
        {
            result->drawLayer = DrawLayers::fromString(drawLayerString);
        }

        return result;
    }

    json toJson(const SpacePortalDefinition &input)
    {
        return json {
            {"id", input.id},
            {"animatedTextureId", input.animatedTextureId},
            {"pullRadius", input.pullRadius},
            {"pullForce", input.pullForce}
        };
    }

    std::unique_ptr<SpacePortalDefinition> fromJsonSpacePortalDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<SpacePortalDefinition>(id);
        j.at("animatedTextureId").get_to(result->animatedTextureId);
        j.at("pullRadius").get_to(result->pullRadius);
        j.at("pullForce").get_to(result->pullForce);

        return result;
    }

    json toJson(const AnimatedTexture &input)
    {
        json sequences;
        for (auto &sequenceKvp : input.sequences())
        {
            sequences[sequenceKvp.first] = toJson(sequenceKvp.second);
        }

        return json {
            {"id", input.id},
            {"texturePath", input.texturePath()},
            {"spriteSize", toJson(input.spriteSize())},
            {"sequences", sequences}
        };
    }

    std::unique_ptr<AnimatedTexture> fromJsonAnimatedTexture(const json &j)
    {
        auto id = j.at("id").get<DefinitionId>();
        auto texturePath = j.at("texturePath").get<std::string>();
        auto spriteSize = fromJsonVector2<ushort>(j.at("spriteSize"));

        auto result = std::make_unique<AnimatedTexture>(id, texturePath, spriteSize);

        auto sequencesJson = j.find("sequences");
        if (sequencesJson != j.end())
        {
            for (auto &sequenceKvp : sequencesJson->items())
            {
                const auto &name = sequenceKvp.key();
                auto sequence = fromJsonAnimationSequence(sequenceKvp.value());

                result->addSequence(name, sequence);
            }
        }

        // return std::move(result);
        return result;
    }

    json toJson(const ShaderDefinition &input)
    {
        return json {
            {"id", input.id},
            {"name", input.name},
            {"fragmentPath", input.fragementPath},
            {"vertexPath", input.vertexPath}
        };
    }

    std::unique_ptr<ShaderDefinition> fromJsonShaderDefinition(const json &j)
    {
        auto id = j.at("id").get<DefinitionId>();
        auto result = std::make_unique<ShaderDefinition>(id);

        j.at("fragmentPath").get_to(result->fragementPath);
        j.at("vertexPath").get_to(result->vertexPath);
        Utils::json_try_get(j, "name", result->name);

        return result;
    }

    json toJson(const GrassEffectDefinition &input)
    {
        return json {
            {"id", input.id},
            {"texturePath", input.texturePath},
            {"shaderId", input.shaderId},
            {"tipColour", Utils::toHexString(input.tipColour)},
            {"sideColour", Utils::toHexString(input.sideColour)},
            {"windColour", Utils::toHexString(input.windColour)}
        };
    }
    std::unique_ptr<GrassEffectDefinition> fromJsonGrassEffectDefinition(const json &j)
    {
        auto id = j.at("id").get<DefinitionId>();
        auto result = std::make_unique<GrassEffectDefinition>(id);

        j.at("texturePath").get_to(result->texturePath);
        j.at("shaderId").get_to(result->shaderId);

        result->tipColour = Utils::fromHexString(j.at("tipColour").get<std::string>());
        result->sideColour = Utils::fromHexString(j.at("sideColour").get<std::string>());
        result->windColour = Utils::fromHexString(j.at("windColour").get<std::string>());

        return result;
    }

    json toJson(const CompendiumDefinition &input)
    {
        return json {
            {"id", input.id},
            {"type", input.type()},
            {"name", input.name},
            {"description", input.description}
        };
    }
    std::unique_ptr<CompendiumDefinition> fromJsonCompendiumDefinition(const json &j)
    {
        auto id = j.at("id").get<DefinitionId>();
        auto result = std::make_unique<CompendiumDefinition>(id);

        j.at("name").get_to(result->name);
        j.at("description").get_to(result->description);

        return result;
    }

    json toJson(const Cursor &input)
    {
        return json {
            {"id", input.id},
            {"name", input.name},
            {"texturePath", input.texturePath},
            {"textureOrigin", toJson(input.textureOrigin)}
        };
    }
    std::unique_ptr<Cursor> fromJsonCursor(const json &j)
    {
        auto id = j.at("id").get<DefinitionId>();
        auto result = std::make_unique<Cursor>(id);

        j.at("name").get_to(result->name);
        j.at("texturePath").get_to(result->texturePath);
        Utils::json_try_get(j, "textureOrigin", result->textureOrigin);

        return result;
    }

    json toJson(const DialogueAudio &input)
    {
        return json {
            {"id", input.id},
            {"sounds", toJsonArray(input.sounds())}
        };
    }
    std::unique_ptr<DialogueAudio> fromJsonDialogueAudio(const json &j)
    {
        auto id = j.at("id").get<DefinitionId>();
        DialogueAudio::SoundList sounds;

        auto soundsJson = j.find("sounds");
        for (auto &soundJson : *soundsJson)
        {
            sounds.push_back(fromJsonAudioReference(soundJson));
        }

        return std::make_unique<DialogueAudio>(id, sounds);
    }

    json toJson(const AudioReference &input)
    {
        return json {
            { "filepath", input.audioPath() }
        };
    }

    AudioReference fromJsonAudioReference(const json &j)
    {
        std::string filepath;
        j.at("filepath").get_to(filepath);

        return AudioReference(filepath, nullptr);
    }

    json toJson(const AnimationSequence &input)
    {
        json result {
            {"frameTiming", input.frameTiming},
        };

        auto &frames = input.frames();
        if (input.areFramesSequential())
        {
            result["range"] = json {frames.front(), frames.back()};
        }
        else
        {
            result["frames"] = toJsonArray(frames);
        }

        return result;
    }

    AnimationSequence fromJsonAnimationSequence(const json &j)
    {
        auto frameTiming = j.at("frameTiming").get<float>();
        auto rangeJson = j.find("range");
        if (rangeJson != j.end())
        {
            auto startIndex = (*rangeJson)[0].get<uint>();
            auto endIndex = (*rangeJson)[1].get<uint>();

            return AnimationSequence(startIndex, endIndex, frameTiming);
        }

        std::vector<uint> frames;
        auto framesJson = j.find("frames");
        for (auto &frame : *framesJson)
        {
            frames.push_back(frame.get<uint>());
        }

        return AnimationSequence(frames, frameTiming);
    }

    json toJson(const CelestialBodyLocation &input)
    {
        return json {
            {"distance", input.distance},
            {"influenceRadius", input.influenceRadius},
            {"angle", input.angle}
        };
    }

    CelestialBodyLocation fromJsonCelestialBodyLocation(const json &j)
    {
        CelestialBodyLocation result;
        j.at("distance").get_to(result.distance);
        j.at("influenceRadius").get_to(result.influenceRadius);
        j.at("angle").get_to(result.angle);

        return result;
    }

    json toJson(const StarBackgroundOptions &input)
    {
        return json {
            {"shaderName", input.shaderName},
            {"starColoursPath", input.starColoursPath},
            {"backgroundColour", Utils::toHexString(input.backgroundColour)},
            {"numParticles", input.numParticles},
            {"area", input.area}
        };
    }

    StarBackgroundOptions fromJsonStarBackgroundOptions(const json &j)
    {
        StarBackgroundOptions result;

        Utils::json_try_get(j, "shaderName", result.shaderName);
        Utils::json_try_get(j, "area", result.area);
        Utils::json_try_get(j, "numParticles", result.numParticles);
        Utils::json_try_get(j, "starColoursPath", result.starColoursPath);

        auto backgroundColourString = j.find("backgroundColour");
        if (backgroundColourString != j.end())
        {
            result.backgroundColour = Utils::fromHexString(backgroundColourString->get<std::string>());
        }

        return result;
    }
} // namespace space