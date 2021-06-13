#pragma once

#include "placeable_item.hpp"

namespace space
{
    class FoodItem : public PlaceableItem
    {
        public:
            // Fields

            // Constructor
            FoodItem(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition) { }
            virtual ~FoodItem() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed);

            static const std::string ItemType() { return "food"; }
            virtual std::string type() const { return ItemType(); }
    };
} // space