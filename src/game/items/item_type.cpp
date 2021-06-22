#include "item_type.hpp"

namespace space
{
    const std::string toString(ItemType2 input)
    {
        switch (input)
        {
            default:
            case ItemType2::Unknown: return "unknown";
            case ItemType2::Placeable: return "placeable-item";
            case ItemType2::Bed: return "bed";
            case ItemType2::Chair: return "chair";
            case ItemType2::Food: return "food";
            case ItemType2::Teleporter: return "teleporter";
        }
    }

    ItemType2 fromStringItemType(const std::string &input)
    {
        if (input == "placeable-item") return ItemType2::Placeable;
        if (input == "bed") return ItemType2::Bed;
        if (input == "chair") return ItemType2::Chair;
        if (input == "food") return ItemType2::Food;
        if (input == "teleporter") return ItemType2::Teleporter;

        return ItemType2::Unknown;
    }
} // namespace space
