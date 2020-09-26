#pragma once

#include <string>

#include "../imgui/imgui.h"

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIPanel : public UIWindow
    {
        public:
            // Fields
            ImVec2 position;
            ImVec2 size;

            // Constructor
            UIPanel(const std::string &windowName);
            virtual ~UIPanel() { }

            // Methods
            virtual void draw(Engine &engine);

        protected:
            // Methods
            virtual void checkPosition(Engine &engine) { }
    };
} // space