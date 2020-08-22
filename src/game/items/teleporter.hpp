#pragma once

#include <string>

#include "placeable_item.hpp"

namespace space
{
    class Teleporter : public PlaceableItem
    {
        public:
            // Fields

            // Constructor
            Teleporter(ItemId id, const PlaceableItemDefinition &definition) : PlaceableItem(id, definition) { }
            virtual ~Teleporter() { }

            // Methods
            virtual void execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea);
            virtual void onPlayerLeaves(GameSession &session);

            const std::string &name() const { return _name; }
            void name(const std::string &name) { _name = name; }

            static const std::string ItemType() { return "teleporter"; }
            virtual std::string type() const { return ItemType(); }

        private:
            std::string _name;
    };
} // space