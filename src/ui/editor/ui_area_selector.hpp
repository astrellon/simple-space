#pragma once

#include "../ui_window.hpp"

namespace space
{
    class Engine;
    class Area;

    class UIAreaSelector : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIAreaSelector();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);
    };
} // space