#pragma once

#include "item.hpp"
#include "../../definitions/placeable_item_definition.hpp"

namespace space
{
    class WalkableArea;
    class GameSession;
    class PlacedItem;

    class PlaceableItem : public Item
    {
        public:
            // Fields
            const PlaceableItemDefinition &placeableDefinition;

            // Constructor
            PlaceableItem(ItemId id, const PlaceableItemDefinition &definition) : Item(id, definition), placeableDefinition(definition), _playerInRange(false) { }
            virtual ~PlaceableItem() { }

            // Methods
            static const std::string ItemType() { return "placeable-item"; }
            virtual std::string type() const { return ItemType(); }

            virtual void execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea) { }
            virtual void onPlaced(PlacedItem &area) { }

            virtual void onPlayerEnters(GameSession &session) { _playerInRange = true; }
            virtual void onPlayerLeaves(GameSession &session) { _playerInRange = false; }

            bool isPlayerInRange() const { return _playerInRange; }

        protected:
            bool _playerInRange;

    };
} // space