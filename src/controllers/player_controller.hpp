#pragma once

#include <SFML/System.hpp>

#include "../types.hpp"
#include "character_controller.hpp"

namespace space
{
    class PlayerController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            PlayerController(GameSession &session);

            // Methods
            static const std::string ControllerType() { return "player"; }
            virtual std::string type() const { return ControllerType(); }

            virtual void update(sf::Time dt);

            void selectedObject(const ObjectId &id) { _selectedObject = id; }
            ObjectId selectedObject() const { return _selectedObject; }

        private:
            // Fields
            ObjectId _selectedObject;

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space