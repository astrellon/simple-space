#pragma once

#include <string>

namespace space
{
    enum class ItemType
    {
        Unknown,
        Placeable,
        Bed,
        Chair,
        Food,
        Teleporter
    };

    const std::string toString(ItemType input);
    ItemType fromStringItemType(const std::string &input);
} // namespace space
