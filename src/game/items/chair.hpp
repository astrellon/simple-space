#pragma once

#include "placeable_item.hpp"

namespace space
{
    class Chair : public PlaceableItem
    {
        public:
            // Fields

            // Constructor
            Chair(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition) { }
            virtual ~Chair() { }

            // Methods
            virtual void execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea);
    };
} // space