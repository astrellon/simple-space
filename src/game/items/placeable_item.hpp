#pragma once

#include "item.hpp"

namespace space
{
    class WalkableArea;
    class GameSession;

    class PlaceableItem : public Item
    {
        public:
            // Fields

            // Constructor
            PlaceableItem(ItemId id, const ItemDefinition &definition) : Item(id, definition) { }
            virtual ~PlaceableItem() { }

            // Methods
            virtual void execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea) { }
            virtual void onPlaced() { }

    };
} // space