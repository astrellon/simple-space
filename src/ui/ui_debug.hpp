#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIDebug : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIDebug();

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

    };
} // space