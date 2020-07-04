#pragma once

#include <SFML/System.hpp>

#include "non_copyable.hpp"
#include "game/inventory.hpp"

namespace space
{
    enum ControllingValue { ControlNone, ControlShip, ControlCharacter };

    class Ship;
    class Character;

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

        private:
            // Fields
            Ship *_ship;
            Character *_character;
            ControllingValue _controlling;
            Inventory _inventory;

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space