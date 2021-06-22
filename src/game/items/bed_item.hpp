#pragma once

#include "placeable_item.hpp"

namespace space
{
    class BedItem : public PlaceableItem
    {
        public:
            // Fields
            static const ItemType2 TypeValue;

            // Constructor
            BedItem(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition, TypeValue) { }
            virtual ~BedItem() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed);

    };
} // space