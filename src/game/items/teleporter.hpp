#pragma once

#include <string>

#include "placeable_item.hpp"

namespace space
{
    class Area;
    class PlacedItem;
    class CharacterController;

    class Teleporter : public PlaceableItem
    {
        public:
            // Fields
            static const ItemType TypeValue;

            // Constructor
            Teleporter(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition, TypeValue) { }
            virtual ~Teleporter() { }

            // Methods
            virtual void execute(CharacterController &controller, PlacedItem &placed);
            virtual void onPlaced(PlacedItem &placedItem);
            virtual void onCharacterLeaves(CharacterController *controller, GameSession &session);

            const std::string &name() const { return _name; }
            void name(const std::string &name) { _name = name; }

        private:
            std::string _name;
    };
} // space