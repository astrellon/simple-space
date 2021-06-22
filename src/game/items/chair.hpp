#pragma once

#include "placeable_item.hpp"

namespace space
{
    class Area;
    class CharacterController;

    class Chair : public PlaceableItem
    {
        public:
            // Fields
            static const ItemType TypeValue;

            // Constructor
            Chair(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition, TypeValue) { }
            virtual ~Chair() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed);
    };
} // space