#pragma once

#include <SFML/System.hpp>

namespace space
{
    enum ControllingValue { ControlNone, ControlShip, ControlCharacter };

    class Ship;
    class Character;

    class PlayerController
    {
        public:
            // Fields

            // Constructor
            PlayerController();

            // Methods
            void update(sf::Time dt);

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

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space