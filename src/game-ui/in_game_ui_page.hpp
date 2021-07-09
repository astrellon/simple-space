#pragma once

#include <string>

#include "./ui_element.hpp"

namespace space
{
    class GameUIInventoryWindow;
    class GameUIInteractablesPanel;
    class GameUITeleportersPanel;
    class GameUIDialogue;
    class GameUIStopFlying;
    class InGameMainMenuPanel;

    class InGameUIPage : public UIElement
    {
        public:
            // Fields

            // Constructor
            InGameUIPage();
            virtual ~InGameUIPage() { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            virtual const char*elementType() const { return "InGameUIPage"; }

            GameUIInteractablesPanel &interactablesPanel() { return *_interactablesPanel; }
            GameUITeleportersPanel &teleportersPanel() { return *_teleportersPanel; }
            GameUIInventoryWindow &inventoryWindow() { return *_inventoryWindow; }
            GameUIDialogue &dialogue() { return *_dialogue; }
            InGameMainMenuPanel &inGameMainMenuPanel() { return *_inGameMainMenuPanel; }

        private:
            // Fields
            GameUIInteractablesPanel *_interactablesPanel;
            GameUITeleportersPanel *_teleportersPanel;
            GameUIInventoryWindow *_inventoryWindow;
            GameUIDialogue *_dialogue;
            GameUIStopFlying *_stopFlying;
            InGameMainMenuPanel *_inGameMainMenuPanel;

            // Methods
    };
} // space