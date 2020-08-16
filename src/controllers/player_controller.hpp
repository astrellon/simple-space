#pragma once

#include <SFML/System.hpp>

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

        private:
            // Fields

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space