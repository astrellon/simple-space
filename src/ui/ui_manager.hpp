#pragma once

#include <memory>
#include <vector>

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIManager
    {
        public:
            // Fields

            // Constructor

            // Methods
            template <typename T, typename... TArgs>
            auto createWindow(TArgs &&... args)
            {
                // We want the return type to stay as type T and not SpaceObject, so we use this round-about way.
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _uiWindows.emplace_back(std::move(obj));

                return result;
            }

            void draw(Engine &engine);

        private:
            // Fields
            std::vector<std::unique_ptr<UIWindow>> _uiWindows;

            // Methods
    };
} // space