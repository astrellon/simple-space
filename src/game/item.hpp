#pragma once

#include <string>

#include "../definitions/item_definition.hpp"

namespace space
{
    class Item
    {
        public:
            // Fields
            const ItemDefinition &definition;

            // Constructor
            Item(const ItemDefinition &definition) : definition(definition) { }
            virtual ~Item() { }

            // Methods
    };
} // space