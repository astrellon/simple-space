#pragma once

#include "../../definitions/base_definition.hpp"
#include "../../definitions/ship_definition.hpp"
#include "json.hpp"

using nlohmann::json;

namespace space
{
    json to_json(const BaseDefinition &input);
    std::unique_ptr<BaseDefinition> from_json_base_definition(const json &j);

    json to_json(const ShipDefinition &input);
    std::unique_ptr<ShipDefinition> from_json_ship_definition(const json &j);
} // namespace space