#include "in_game_ui_page.hpp"

#include "./in-game/game_ui_interactables_panel.hpp"
#include "./in-game/game_ui_teleporters_panel.hpp"
#include "./in-game/game_ui_inventory_window.hpp"
#include "./in-game/game_ui_dialogue.hpp"

#include "./game_ui_manager.hpp"

namespace space
{
    InGameUIPage::InGameUIPage() :
        _interactablesPanel(nullptr),
        _teleportersPanel(nullptr),
        _inventoryWindow(nullptr),
        _dialogue(nullptr)
    {

    }

    void InGameUIPage::init(GameUIManager &uiManager)
    {
        _inventoryWindow = uiManager.createElement<GameUIInventoryWindow>();
        addChild(_inventoryWindow);

        _interactablesPanel = uiManager.createElement<GameUIInteractablesPanel>();
        addChild(_interactablesPanel);

        _teleportersPanel = uiManager.createElement<GameUITeleportersPanel>();
        addChild(_teleportersPanel);

        _dialogue = uiManager.createElement<GameUIDialogue>();
        addChild(_dialogue);
    }
} // space