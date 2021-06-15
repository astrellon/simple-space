#pragma once

#include "placeable_item.hpp"

namespace space
{
    class BedItem : public PlaceableItem
    {
        public:
            // Fields

            // Constructor
            BedItem(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition) { }
            virtual ~BedItem() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed);

            static const std::string ItemType() { return "bed"; }
            virtual std::string type() const { return ItemType(); }
    };
} // space