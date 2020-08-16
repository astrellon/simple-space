#include "json_serialisers_definitions.hpp"

#include <iostream>
#include <stdexcept>

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

using nlohmann::json;

namespace space
{
    json toJson(const BaseDefinition &input)
    {
        json j;
        auto type = input.type();
        if (type == ShipDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const ShipDefinition &>(input));
        }
        else if (type == CharacterDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const CharacterDefinition &>(input));
        }
        else if (type == PlanetDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const PlanetDefinition &>(input));
        }
        else if (type == OrbitPointCelestialDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const OrbitPointCelestialDefinition &>(input));
        }
        else if (type == StarSystemDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const StarSystemDefinition &>(input));
        }
        else if (type == PlanetSurfaceDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const PlanetSurfaceDefinition &>(input));
        }
        else if (type == Dialogue::DefinitionType())
        {
            j = toJson(dynamic_cast<const Dialogue &>(input));
        }
        else
        {
            std::cout << "Error!" << std::endl;
        }

        j["type"] = type;
        return j;
    }

    std::unique_ptr<BaseDefinition> fromJsonBaseDefinition(const json &j)
    {
        auto type = j.at("type").get<std::string>();

        if (type == ShipDefinition::DefinitionType())
        {
            return fromJsonShipDefinition(j);
        }
        else if (type == CharacterDefinition::DefinitionType())
        {
            return fromJsonCharacterDefinition(j);
        }
        else if (type == PlanetDefinition::DefinitionType() ||
            type == OrbitPointCelestialDefinition::DefinitionType())
        {
            return fromJsonCelestialBodyDefinition(j);
        }
        else if (type == StarSystemDefinition::DefinitionType())
        {
            return fromJsonStarSystemDefinition(j);
        }
        else if (type == PlanetSurfaceDefinition::DefinitionType())
        {
            return fromJsonPlanetSurfaceDefinition(j);
        }
        else if (type == Dialogue::DefinitionType())
        {
            return fromJsonDialogue(j);
        }

        throw std::runtime_error("Oh no");
    }

    json toJson(const ShipDefinition &input)
    {
        json interiorPolygon, enginePlacements;
        for (auto &pos : input.interiorPolygon)
            interiorPolygon.push_back(json { pos.x, pos.y });

        for (auto &pos : input.enginePlacements)
            enginePlacements.push_back(json { pos.x, pos.y });

        return json {
            {"id", input.id},
            {"texturePath", input.texturePath},
            {"interiorTexturePath", input.interiorTexturePath},
            {"interiorTextureOffset", json {
                input.interiorTextureOffset.x, input.interiorTextureOffset.y}
            },
            {"engineGlowTexturePath", input.engineGlowTexturePath},
            {"name", input.name},
            {"maxRotation", input.maxRotation},
            {"maxSpeed", input.maxSpeed},
            {"turnRate", input.turnRate},
            {"acceleration", input.acceleration},
            {"interiorPolygon", interiorPolygon},
            {"enginePlacements", enginePlacements}
        };
    }

    std::unique_ptr<ShipDefinition> fromJsonShipDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto result = std::make_unique<ShipDefinition>(id);
        j.at("texturePath").get_to(result->texturePath);
        j.at("interiorTexturePath").get_to(result->interiorTexturePath);

        auto interiorTextureOffset = j.find("interiorTextureOffset");
        if (interiorTextureOffset != j.end())
        {
            (*interiorTextureOffset)[0].get_to(result->interiorTextureOffset.x);
            (*interiorTextureOffset)[1].get_to(result->interiorTextureOffset.y);
        }

        auto interiorPolygon = j.find("interiorPolygon");
        if (interiorPolygon != j.end())
        {
            for (auto &child : *interiorPolygon)
            {
                sf::Vector2f pos(child[0].get<float>(), child[1].get<float>());
                result->interiorPolygon.push_back(pos);
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

        Utils::json_try_set(j, "engineGlowTexturePath", result->engineGlowTexturePath);
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
            {"texturePath", input.texturePath},
            {"name", input.name},
            {"spriteSize", input.spriteSize},
            {"speed", input.speed},
        };
    }

    std::unique_ptr<CharacterDefinition> fromJsonCharacterDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto input = std::make_unique<CharacterDefinition>(id);
        j.at("texturePath").get_to(input->texturePath);
        j.at("name").get_to(input->name);
        j.at("spriteSize").get_to(input->spriteSize);
        j.at("speed").get_to(input->speed);

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
        json childrenJson;
        for (auto &child : input.children)
        {
            childrenJson.push_back(toJson(*child.get()));
        }

        return json {
            {"id", input.id},
            {"location", toJson(input.location)},
            {"children", childrenJson}
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
        json childrenJson;
        for (auto &child : input.children)
        {
            childrenJson.push_back(toJson(*child.get()));
        }

        return json
        {
            {"id", input.id},
            {"location", toJson(input.location)},
            {"children", childrenJson},
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

        Utils::json_try_set(j, "oscillateNoise", result->oscillateNoise);

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

        return result;
    }

    json toJson(const Dialogue &input)
    {
        json textJson = {};
        for (auto &line : input.text)
        {
            textJson.push_back(line);
        }

        return json {
            {"id", input.id},
            {"text", textJson}
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
            {"backgroundColour", Utils::toHexString(input.backgroundColour)},
            {"numLayers", input.numLayers},
            {"numParticles", input.numParticles},
            {"area", input.area}
        };
    }

    StarBackgroundOptions fromJsonStarBackgroundOptions(const json &j)
    {
        StarBackgroundOptions result;

        Utils::json_try_set(j, "shaderName", result.shaderName);
        Utils::json_try_set(j, "area", result.area);
        Utils::json_try_set(j, "numLayers", result.numLayers);
        Utils::json_try_set(j, "numParticles", result.numParticles);

        auto backgroundColourString = j.find("backgroundColour");
        if (backgroundColourString != j.end())
        {
            result.backgroundColour = Utils::fromHexString(backgroundColourString->get<std::string>());
        }

        return result;
    }
} // namespace space