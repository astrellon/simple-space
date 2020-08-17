#pragma once

#include <string>

#include "../../non_copyable.hpp"
#include "../../definitions/item_definition.hpp"

namespace space
{
    typedef int ItemId;

    class Item : private NonCopyable
    {
        public:
            // Fields
            const ItemId id;
            const ItemDefinition &definition;

            // Constructor
            Item(ItemId id, const ItemDefinition &definition) : id(id), definition(definition) { }
            virtual ~Item() { }

            // Methods
            virtual std::string type() const = 0;
    };
} // space