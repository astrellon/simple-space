#pragma once

#include <string>
#include <observe/event.h>

namespace space
{
    class Dialogue;
    class DialogueAudio;

    class DialogueManager
    {
        public:
            typedef observe::Event<> NextDialogueHandler;

            // Fields
            NextDialogueHandler onNextDialogue;

            // Constructor
            DialogueManager();

            // Methods
            void startDialogue(const std::string &personTalking, const Dialogue *dialogue, const DialogueAudio *dialogueAudio);
            bool tryGetNextLine(std::string *result);
            bool nextLine();
            const std::string &personTalkingName() const { return _personTalking; }
            const DialogueAudio *dialogueAudio() const { return _dialogueAudio; }

            bool isInDialogue() const { return _inDialogue; }
            const std::string &currentLine() const { return _currentLine; }

        private:
            // Fields
            std::string _personTalking;
            const Dialogue *_current;
            const DialogueAudio *_dialogueAudio;
            bool _inDialogue;
            int _nextPage;
            std::string _currentLine;

            // Methods
    };
} // space