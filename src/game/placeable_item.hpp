#pragma once

#include "item.hpp"

namespace space
{
    class PlaceableItem : public Item
    {
        public:
            // Fields

            // Constructor
            PlaceableItem(const ItemDefinition &definition) : Item(definition) { }
            virtual ~PlaceableItem() { }

            // Methods
            virtual void execute() { }
            virtual void onPlaced() { }
    };
} // space