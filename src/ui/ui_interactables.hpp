#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIInteractables : public UIWindow
    {
        public:
            // Constructors
            UIInteractables();

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void checkPosition(Engine &engine);
            virtual void doDraw(Engine &engine);
    };
} // space