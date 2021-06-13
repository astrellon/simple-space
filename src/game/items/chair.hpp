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

            // Constructor
            Chair(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition) { }
            virtual ~Chair() { }

            // Methods
            virtual void execute(CharacterController *controller, GameSession &session, const sf::Vector2f &position, Area &parentArea);

            static const std::string ItemType() { return "chair"; }
            virtual std::string type() const { return ItemType(); }
    };
} // space