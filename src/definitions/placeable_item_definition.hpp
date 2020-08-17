#pragma once

#include "item_definition.hpp"

namespace space
{
    class PlaceableItemDefinition : public ItemDefinition
    {
        public:
            // Fields
            bool canPickup;

            // Constructor
            PlaceableItemDefinition(DefinitionId id) : ItemDefinition(id) { }
            virtual ~PlaceableItemDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "placeable-item"; }
            virtual std::string type() const { return DefinitionType(); }
    };
} // space