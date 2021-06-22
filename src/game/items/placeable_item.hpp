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
            static const ItemType TypeValue;

            const PlaceableItemDefinition &placeableDefinition;

            // Constructor
            PlaceableItem(ItemId id, const PlaceableItemDefinition &definition, ItemType t = TypeValue) : Item(id, definition, t), placeableDefinition(definition) { }
            virtual ~PlaceableItem() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed) { }
            virtual void onPlaced(PlacedItem &area) { }

            virtual void onCharacterEnters(CharacterController *controller, GameSession &session) {}
            virtual void onCharacterLeaves(CharacterController *controller, GameSession &session) {}

    };
} // space