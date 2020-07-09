#pragma once

#include <algorithm>
#include <vector>
#include <SFML/System.hpp>

#include "non_copyable.hpp"
#include "game/inventory.hpp"

namespace space
{
    enum ControllingValue { ControlNone, ControlShip, ControlCharacter };

    class Ship;
    class Character;
    class PlacedItem;
    class PlaceableItem;

    class PlayerController : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            PlayerController();

            // Methods
            void update(sf::Time dt);

            Inventory &inventory() { return _inventory; }
            const Inventory &inventory() const { return _inventory; }

            void controlling(ControllingValue value) { _controlling = value; }
            ControllingValue controlling() const { return _controlling; }

            void controllingShip(Ship *ship) { _ship = ship; }
            Ship *controllingShip() const { return _ship; }

            void controllingCharacter(Character *character) { _character = character; }
            Character *controllingCharacter() const { return _character; }

            bool addCanInteractWith(PlacedItem *item);
            bool removeCanInteractWith(PlacedItem *item);
            bool canInteractWith(PlacedItem *item) const
            {
                auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
                return find != _canInteractWith.end();
            }
            std::vector<PlacedItem *> &canInteractWith() { return _canInteractWith; }
            void clearCanInteractWith() { _canInteractWith.clear(); }

            void addShipInTeleportRange(Ship *ship);
            void removeShipInTeleportRange(Ship *ship);
            bool shipInTeleportRange(Ship *ship) const
            {
                auto find = std::find(_shipsInTeleportRange.begin(), _shipsInTeleportRange.end(), ship);
                return find != _shipsInTeleportRange.end();
            }
            std::vector<Ship *> &shipsInTeleportRange() { return _shipsInTeleportRange; }
            void clearShipsInTeleportRange() { _shipsInTeleportRange.clear(); }

            void dropItem(PlaceableItem *placeableItem);

            void interactWithClosest();

        private:
            // Fields
            Ship *_ship;
            Character *_character;
            ControllingValue _controlling;
            Inventory _inventory;
            std::vector<PlacedItem *> _canInteractWith;
            std::vector<Ship *> _shipsInTeleportRange;

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space