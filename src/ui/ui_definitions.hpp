#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIDefinitions : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIDefinitions();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

    };
} // space