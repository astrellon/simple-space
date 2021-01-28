#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;
    class UIElement;

    class UIElementWindow : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIElementWindow();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            UIElement *_selectedElement;

            // Methods
            void drawElement(UIElement &element);
    };
} // space