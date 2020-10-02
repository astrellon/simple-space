#pragma once

#include "character_controller.hpp"

namespace space
{
    class GameSession;

    class BirdController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            BirdController(GameSession &session);

            // Methods
            static const std::string ControllerType() { return "bird"; }
            virtual std::string type() const { return ControllerType(); }

            virtual void update(sf::Time dt);

        private:
            // Fields
            float _timeToBlink;

            // Methods
    };
} // space