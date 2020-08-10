#pragma once

#include <SFML/System.hpp>

#include "character_controller.hpp"

namespace space
{
    class Dialogue;
    class Interaction;

    class NpcController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            NpcController(GameSession &session);

            // Methods
            virtual void update(sf::Time dt);

            void dialogue(const Dialogue *dialogue);
            const Dialogue *dialogue() const { return _dialogue; }

        private:
            // Fields
            const Dialogue *_dialogue;
            Interaction *_startDialogueAction;
    };
} // space