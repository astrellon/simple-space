#include "ui_manager.hpp"

#include "ui_window.hpp"
#include "ui_interactables.hpp"
#include "ui_inventory.hpp"
#include "ui_teleporter.hpp"
#include "ui_dialogue.hpp"
#include "ui_debug.hpp"
#include "ui_definitions.hpp"

namespace space
{
    UIManager::UIManager() : _uiInteractables(nullptr), _uiInventory(nullptr), _uiTeleporter(nullptr), _uiDialogue(nullptr)
    {

    }

    void UIManager::initDefaultWindows()
    {
        _uiInteractables = createWindow<UIInteractables>();
        _uiInventory = createWindow<UIInventory>();
        _uiTeleporter = createWindow<UITeleporter>();
        _uiDialogue = createWindow<UIDialogue>();
        _uiDebug = createWindow<UIDebug>();
        _uiDefinitions = createWindow<UIDefinitions>();
    }

    void UIManager::draw(Engine &engine)
    {
        for (auto &window : _uiWindows)
        {
            window->draw(engine);
        }
    }
} // namespace space
