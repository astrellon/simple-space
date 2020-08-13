#pragma once

#include "ui_window.hpp"

namespace space
{
    class PlayerController;

    class UIInventory : public UIWindow
    {
        public:
            // Constructors
            UIInventory();

        protected:
            // Methods
            virtual void checkOpen(Engine &engine);
            virtual void doDraw(Engine &engine);

    };
} // space