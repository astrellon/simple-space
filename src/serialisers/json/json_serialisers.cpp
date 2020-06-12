#include "json_serialisers.hpp"

#include <iostream>
#include <stdexcept>

#include "../../definitions/base_definition.hpp"
#include "../../definitions/ship_definition.hpp"
#include "json.hpp"

using nlohmann::json;

namespace space
{
    json to_json(const BaseDefinition *input)
    {
        json j;
        auto type = input->type();
        if (type == ShipDefinition::DefinitionType())
        {
            j = to_json(dynamic_cast<const ShipDefinition *>(input));
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

        throw std::runtime_error("Oh no");
    }

    json to_json(const ShipDefinition *input)
    {
        return json {
            {"id", input->id},
            {"texturePath", input->texturePath},
            {"name", input->name},
            {"maxRotation", input->maxRotation},
            {"maxSpeed", input->maxSpeed},
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

        return input;
    }
} // namespace space