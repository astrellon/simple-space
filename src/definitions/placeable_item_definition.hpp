#pragma once

#include "item_definition.hpp"
#include "../physics/physics_shape.hpp"
#include "../game/draw_layers.hpp"

namespace space
{
    class PlaceableItemDefinition : public ItemDefinition
    {
        public:
            // Fields
            bool canPickup;
            bool canUse;
            PhysicsShape physicsShape;
            DrawLayers::Type drawLayer;

            // Constructor
            PlaceableItemDefinition(DefinitionId id) : ItemDefinition(id), canPickup(false), canUse(true), drawLayer(DrawLayers::Main) { }
            virtual ~PlaceableItemDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "placeable-item"; }
            virtual std::string type() const { return DefinitionType(); }
    };
} // space