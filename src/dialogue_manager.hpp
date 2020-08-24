#pragma once

#include <string>

namespace space
{
    class Dialogue;

    class DialogueManager
    {
        public:
            // Fields

            // Constructor
            DialogueManager();

            // Methods
            void startDialogue(const std::string &personTalking, const Dialogue *dialogue);
            bool tryGetNextLine(std::string *result);
            bool nextLine();
            const std::string &personTalkingName() const { return _personTalking; }

            bool isInDialogue() const { return _inDialogue; }
            const std::string &currentLine() const { return _currentLine; }

        private:
            // Fields
            std::string _personTalking;
            const Dialogue *_current;
            bool _inDialogue;
            int _nextPage;
            std::string _currentLine;

            // Methods
    };
} // space