#pragma once

#include "item.hpp"
#include "../../definitions/placeable_item_definition.hpp"

namespace space
{
    class Area;
    class GameSession;
    class PlacedItem;
    class CharacterController;

    class PlaceableItem : public Item
    {
        public:
            // Fields
            const PlaceableItemDefinition &placeableDefinition;

            // Constructor
            PlaceableItem(ItemId id, const PlaceableItemDefinition &definition) : Item(id, definition), placeableDefinition(definition) { }
            virtual ~PlaceableItem() { }

            // Methods
            static const std::string ItemType() { return "placeable-item"; }
            virtual std::string type() const { return ItemType(); }

            virtual void execute(CharacterController *controller, GameSession &session, const sf::Vector2f &position, Area &parentArea) { }
            virtual void onPlaced(PlacedItem &area) { }

            virtual void onCharacterEnters(CharacterController *controller, GameSession &session) {}
            virtual void onCharacterLeaves(CharacterController *controller, GameSession &session) {}

    };
} // space