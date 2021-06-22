#pragma once

#include <string>

#include "../../non_copyable.hpp"
#include "../../definitions/item_definition.hpp"
#include "item_type.hpp"

namespace space
{
    typedef int ItemId;

    class Item : private NonCopyable
    {
        public:
            // Fields
            static const ItemId UnknownId;

            const ItemId id;
            const ItemType type;
            const ItemDefinition &definition;

            // Constructor
            Item(ItemId id, const ItemDefinition &definition, ItemType t) : id(id), type(t), definition(definition) { }
            virtual ~Item() { }

            // Methods
            template <typename T>
            bool tryCast(const T *& result) const
            {
                if (type == T::TypeValue)
                {
                    result = reinterpret_cast<const T *>(this);
                    return true;
                }
                return false;
            }

            template <typename T>
            bool tryCast(T *& result)
            {
                if (type == T::TypeValue)
                {
                    result = reinterpret_cast<T *>(this);
                    return true;
                }
                return false;
            }

            template <typename T>
            bool is() const
            {
                return type == T::TypeValue;
            }

    };
} // space