#pragma once

#include "interaction.hpp"

namespace space
{
    class GameSession;
    class Dialogue;

    class StartDialogueAction : public Interaction
    {
        public:
            // Fields

            // Constructor
            StartDialogueAction(const Dialogue *dialogue);

            // Methods
            virtual std::string label() const { return "Talk"; }
            virtual void execute(GameSession &session);

        private:
            const Dialogue *_dialogue;
    };
} // space