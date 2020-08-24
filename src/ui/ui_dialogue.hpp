#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIDialogue : public UIWindow
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

            // Methods
            void processText(Engine &engine);

    };
} // space