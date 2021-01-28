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
    class UIDebug;
    class UIDefinitions;
    class UISelected;
    class UIObjects;
    class UIFrameBuffers;
    class UIPhotoAlbum;
    class UICompendium;
    class UIElementWindow;

    class UIManager
    {
        public:
            typedef std::vector<std::unique_ptr<UIWindow>>  WindowList;
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

            WindowList &windows() { return _uiWindows; }

            UIInteractables &uiInteractables() { return *_uiInteractables; }
            UIInventory &uiInventory() { return *_uiInventory; }
            UITeleporter &uiTeleporter() { return *_uiTeleporter; }
            UIDialogue &uiDialogue() { return *_uiDialogue; }
            UIDebug &uiDebug() { return *_uiDebug; }
            UIDefinitions &uiDefinitions() { return *_uiDefinitions; }
            UISelected &uiSelected() { return *_uiSelected; }
            UIObjects &uiObjects() { return *_uiObjects; }
            UIFrameBuffers &uiFrameBuffers() { return *_uiFrameBuffers; }
            UIPhotoAlbum &uiPhotoAlbum() { return *_uiPhotoAlbum; }
            UICompendium &uiCompendium() { return *_uiCompendium; }
            UIElementWindow &uiElementWindow() { return *_uiElementWindow; }

        private:
            // Fields
            WindowList _uiWindows;

            UIInteractables *_uiInteractables;
            UIInventory *_uiInventory;
            UITeleporter *_uiTeleporter;
            UIDialogue *_uiDialogue;
            UIDebug *_uiDebug;
            UIDefinitions *_uiDefinitions;
            UISelected *_uiSelected;
            UIObjects *_uiObjects;
            UIFrameBuffers *_uiFrameBuffers;
            UIPhotoAlbum *_uiPhotoAlbum;
            UICompendium *_uiCompendium;
            UIElementWindow *_uiElementWindow;

            // Methods
    };
} // space