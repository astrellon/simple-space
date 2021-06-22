#pragma once

#include "placeable_item.hpp"

namespace space
{
    class FoodItem : public PlaceableItem
    {
        public:
            // Fields
            static const ItemType TypeValue;

            // Constructor
            FoodItem(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition, TypeValue) { }
            virtual ~FoodItem() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed);
    };
} // space