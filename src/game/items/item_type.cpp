#include "item_type.hpp"

namespace space
{
    const std::string toString(ItemType input)
    {
        switch (input)
        {
            default:
            case ItemType::Unknown: return "unknown";
            case ItemType::Placeable: return "placeable-item";
            case ItemType::Bed: return "bed";
            case ItemType::Chair: return "chair";
            case ItemType::Food: return "food";
            case ItemType::Teleporter: return "teleporter";
        }
    }

    ItemType fromStringItemType(const std::string &input)
    {
        if (input == "placeable-item") return ItemType::Placeable;
        if (input == "bed") return ItemType::Bed;
        if (input == "chair") return ItemType::Chair;
        if (input == "food") return ItemType::Food;
        if (input == "teleporter") return ItemType::Teleporter;

        return ItemType::Unknown;
    }
} // namespace space
