#pragma once

#include "interaction.hpp"

namespace space
{
    class GameSession;
    class Dialogue;
    class DialogueAudio;

    class StartDialogueAction : public Interaction
    {
        public:
            // Fields

            // Constructor
            StartDialogueAction(const std::string &personTalking, const Dialogue *dialogue, const DialogueAudio *dialogueAudio);

            // Methods
            virtual std::string label() const { return "Talk"; }
            virtual void execute(GameSession &session);

        private:
            std::string _personTalking;
            const Dialogue *_dialogue;
            const DialogueAudio *_dialogueAudio;
    };
} // space