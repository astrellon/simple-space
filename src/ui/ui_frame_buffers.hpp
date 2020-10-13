#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIFrameBuffers : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIFrameBuffers();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);
    };
} // space