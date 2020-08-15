#pragma once

#include <memory>
#include <vector>

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIInteractables;
    class UIInventory;
    class UITeleporter;
    class UIDialogue;

    class UIManager
    {
        public:
            // Fields

            // Constructor
            UIManager();

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

            void initDefaultWindows();
            void draw(Engine &engine);

            UIInteractables &uiInteractables() { return *_uiInteractables; }
            UIInventory &uiInventory() { return *_uiInventory; }
            UITeleporter &uiTeleporter() { return *_uiTeleporter; }
            UIDialogue &uiDialogue() { return *_uiDialogue; }

        private:
            // Fields
            std::vector<std::unique_ptr<UIWindow>> _uiWindows;
            UIInteractables *_uiInteractables;
            UIInventory *_uiInventory;
            UITeleporter *_uiTeleporter;
            UIDialogue *_uiDialogue;

            // Methods
    };
} // space