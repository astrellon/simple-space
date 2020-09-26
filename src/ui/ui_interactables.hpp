#pragma once

#include "ui_panel.hpp"

namespace space
{
    class Engine;

    class UIInteractables : public UIPanel
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