#pragma once

#include "../../definitions/base_definition.hpp"
#include "../../definitions/ship_definition.hpp"
#include "json.hpp"

using nlohmann::json;

namespace space
{
    json toJson(const BaseDefinition &input);
    std::unique_ptr<BaseDefinition> fromJsonBaseDefinition(const json &j);

    json toJson(const ShipDefinition &input);
    std::unique_ptr<ShipDefinition> fromJsonShipDefinition(const json &j);
} // namespace space