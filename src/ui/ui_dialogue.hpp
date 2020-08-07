#pragma once

namespace space
{
    class DialogueManager;

    class UIDialogue
    {
        public:
            static void draw(DialogueManager &manager);

        private:
            UIDialogue();
    };
} // space