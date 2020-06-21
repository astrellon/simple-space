#include "json_serialisers.hpp"

#include <iostream>
#include <stdexcept>

#include "utils.hpp"

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
        else if (type == PlanetDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const PlanetDefinition &>(input));
        }
        else if (type == OrbitPointCelestialDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const OrbitPointCelestialDefinition &>(input));
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
        else if (type == PlanetDefinition::DefinitionType() ||
            type == OrbitPointCelestialDefinition::DefinitionType())
        {
            return fromJsonCelestialBodyDefinition(j);
        }

        throw std::runtime_error("Oh no");
    }

    json toJson(const ShipDefinition &input)
    {
        return json {
            {"id", input.id},
            {"texturePath", input.texturePath},
            {"name", input.name},
            {"maxRotation", input.maxRotation},
            {"maxSpeed", input.maxSpeed},
            {"turnRate", input.turnRate},
            {"acceleration", input.acceleration},
        };
    }

    std::unique_ptr<ShipDefinition> fromJsonShipDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto input = std::make_unique<ShipDefinition>(id);
        j.at("texturePath").get_to(input->texturePath);
        j.at("name").get_to(input->name);
        j.at("maxRotation").get_to(input->maxRotation);
        j.at("maxSpeed").get_to(input->maxSpeed);
        j.at("turnRate").get_to(input->turnRate);
        j.at("acceleration").get_to(input->acceleration);

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
            for (auto &childJson : j)
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
            for (auto &childJson : j)
            {
                result->children.emplace_back(std::move(fromJsonCelestialBodyDefinition(childJson)));
            }
        }

        j.at("texturePath").get_to(result->texturePath);
        j.at("name").get_to(result->name);
        j.at("rotationRate").get_to(result->rotationRate);
        j.at("size").get_to(result->size);
        j.at("scale").get_to(result->scale);

        auto oscillateNoise = j.find("oscillateNoise");
        if (oscillateNoise != j.end())
        {
            oscillateNoise->get_to(result->oscillateNoise);
        }

        result->glowColour = Utils::fromHexString(j.at("glowColour").get<std::string>());

        return result;
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
} // namespace space