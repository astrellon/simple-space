#pragma once

#include <string>
#include <vector>

#include "ui_panel.hpp"

namespace space
{
    class Engine;

    class UIDialogue : public UIPanel
    {
        public:
            // Constructors
            UIDialogue();

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void checkPosition(Engine &engine);
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            std::string _text;
            std::vector<char> _textDisplay;
            size_t _textDisplayIndex;

            // Methods
            void processText(Engine &engine);

    };
} // space