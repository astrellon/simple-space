#pragma once

#include <string>

#include "../imgui/imgui.h"

#include "../non_copyable.hpp"

namespace space
{
    class Engine;

    class UIWindow : private NonCopyable
    {
        public:
            // Fields
            ImVec2 position;
            ImVec2 size;
            bool isOpen;
            const std::string windowName;

            // Constructor
            UIWindow(const std::string &windowName);

            // Methods
            virtual void draw(Engine &engine);

        protected:
            // Methods
            virtual void checkOpen(Engine &engine) { }
            virtual void checkPosition(Engine &engine) { }
            virtual void doDraw(Engine &engine) = 0;

    };
} // space