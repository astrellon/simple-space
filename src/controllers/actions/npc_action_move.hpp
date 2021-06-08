#pragma once

#include <SFML/System.hpp>

#include "npc_action.hpp"

namespace space
{
    class NpcActionMove : public NpcAction
    {
        public:
            // Fields

            // Constructor
            NpcActionMove(NpcController *controller, sf::Vector2f destination);

            // Methods
            static const std::string ActionType() { return "move-action"; }
            virtual std::string type() const { return ActionType(); }

            virtual void update(sf::Time dt);
            virtual bool isComplete() const;

            virtual void onComplete();

        private:
            // Fields
            sf::Vector2f _destination;
            float _toDestination;

            // Methods
    };
} // space