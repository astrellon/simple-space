#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UICompendium : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UICompendium();

            // Methods

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void doDraw(Engine &engine);
    };
} // space