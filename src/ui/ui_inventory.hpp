#pragma once

#include "ui_panel.hpp"

namespace space
{
    class PlayerController;

    class UIInventory : public UIPanel
    {
        public:
            // Constructors
            UIInventory();

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void doDraw(Engine &engine);

    };
} // space