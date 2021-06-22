#pragma once

#include <string>

namespace space
{
    enum class ItemType2
    {
        Unknown,
        Placeable,
        Bed,
        Chair,
        Food,
        Teleporter
    };

    const std::string toString(ItemType2 input);
    ItemType2 fromStringItemType(const std::string &input);
} // namespace space
