#pragma once

#include "item.hpp"

namespace space
{
    class PlaceableItem : public Item
    {
        public:
            // Fields

            // Constructor
            PlaceableItem(ItemId id, const ItemDefinition &definition) : Item(id, definition) { }
            virtual ~PlaceableItem() { }

            // Methods
            virtual void execute() { }
            virtual void onPlaced() { }
    };
} // space