#pragma once

#include "../ui_window.hpp"

namespace space
{
    class Engine;

    class UIEditorMenu : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIEditorMenu();

            // Methods
            virtual void draw(Engine &engine);

        protected:
            // Methods
            virtual void doDraw(Engine &engine);
    };
} // space