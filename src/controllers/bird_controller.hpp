#pragma once

#include "npc_controller.hpp"

namespace space
{
    class GameSession;

    class BirdController : public NpcController
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
            float _timeToNextAction;

            // Methods
    };
} // space