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
            const std::string windowName;
            bool show;

            // Constructor
            UIWindow(const std::string &windowName);
            virtual ~UIWindow() { }

            // Methods
            virtual void draw(Engine &engine);
            virtual bool toggleShow() { return show = !show; }

        protected:
            // Methods
            virtual bool isOpen(Engine &engine) { return show; }
            virtual void doDraw(Engine &engine) = 0;

    };
} // space