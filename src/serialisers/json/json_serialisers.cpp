#include "json_serialisers.hpp"

#include <iostream>
#include <stdexcept>

#include "utils.hpp"

using nlohmann::json;

namespace space
{
    json toJson(const BaseDefinition *input)
    {
        json j;
        auto type = input->type();
        if (type == ShipDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const ShipDefinition *>(input));
        }
        else if (type == PlanetDefinition::DefinitionType())
        {
            j = toJson(dynamic_cast<const PlanetDefinition *>(input));
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
        else if (type == PlanetDefinition::DefinitionType())
        {
            return fromJsonPlanetDefinition(j);
        }

        throw std::runtime_error("Oh no");
    }

    json toJson(const ShipDefinition *input)
    {
        return json {
            {"id", input->id},
            {"texturePath", input->texturePath},
            {"name", input->name},
            {"maxRotation", input->maxRotation},
            {"maxSpeed", input->maxSpeed},
            {"turnRate", input->turnRate},
            {"acceleration", input->acceleration},
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

    json toJson(const PlanetDefinition *input)
    {
        return json {
            {"id", input->id},
            {"texturePath", input->texturePath},
            {"name", input->name},
            {"rotationRate", input->rotationRate},
            {"size", input->size},
            {"scale", input->scale},
            {"glowColour", Utils::toHexString(input->glowColour)}
        };
    }

    std::unique_ptr<PlanetDefinition> fromJsonPlanetDefinition(const json &j)
    {
        auto id = j.at("id").get<std::string>();
        auto input = std::make_unique<PlanetDefinition>(id);
        j.at("texturePath").get_to(input->texturePath);
        j.at("name").get_to(input->name);
        j.at("rotationRate").get_to(input->rotationRate);
        j.at("size").get_to(input->size);
        j.at("scale").get_to(input->scale);

        auto oscillateNoise = j.find("oscillateNoise");
        if (oscillateNoise != j.end())
        {
            oscillateNoise->get_to(input->oscillateNoise);
        }

        input->glowColour = Utils::fromHexString(j.at("glowColour").get<std::string>());

        return input;
    }
} // namespace space