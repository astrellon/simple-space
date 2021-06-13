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

            // Constructor
            Teleporter(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition) { }
            virtual ~Teleporter() { }

            // Methods
            virtual void execute(CharacterController *controller, GameSession &session, const sf::Vector2f &position, Area &parentArea);
            virtual void onPlaced(PlacedItem &placedItem);
            virtual void onCharacterLeaves(CharacterController *controller, GameSession &session);

            const std::string &name() const { return _name; }
            void name(const std::string &name) { _name = name; }

            static const std::string ItemType() { return "teleporter"; }
            virtual std::string type() const { return ItemType(); }

        private:
            std::string _name;
    };
} // space