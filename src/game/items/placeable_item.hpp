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
            PlaceableItem(ItemId id, const ItemDefinition &definition) : Item(id, definition), _playerInRange(false) { }
            virtual ~PlaceableItem() { }

            // Methods
            static const std::string ItemType() { return "placeable-item"; }
            virtual std::string type() const { return ItemType(); }

            virtual void execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea) { }
            virtual void onPlaced() { }

            virtual void onPlayerEnters(GameSession &session) { _playerInRange = true; }
            virtual void onPlayerLeaves(GameSession &session) { _playerInRange = false; }

            bool isPlayerInRange() const { return _playerInRange; }

        protected:
            bool _playerInRange;

    };
} // space