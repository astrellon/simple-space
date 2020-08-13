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
            virtual void checkOpen(Engine &engine);
            virtual void checkPosition(Engine &engine);
            virtual void doDraw(Engine &engine);

    };
} // space