#pragma once

#include <SFML/System.hpp>

#include "character_controller.hpp"

namespace space
{
    class Dialogue;
    class Interaction;

    class SpaceStationController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            SpaceStationController(GameSession &session);

            // Methods
            static const std::string ControllerType() { return "space-station"; }
            virtual std::string type() const { return ControllerType(); }

            virtual void update(sf::Time dt);

        private:
            // Fields
    };
} // space