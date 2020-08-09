#pragma once

#include <SFML/System.hpp>

#include "character_controller.hpp"

namespace space
{
    class NpcController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            NpcController(GameSession &session);

            // Methods
            virtual void update(sf::Time dt);
    };
} // space