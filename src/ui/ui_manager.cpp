#include "ui_manager.hpp"

#include "ui_window.hpp"
#include "ui_interactables.hpp"
#include "ui_inventory.hpp"
#include "ui_teleporter.hpp"
#include "ui_dialogue.hpp"

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
    }

    void UIManager::draw(Engine &engine)
    {
        for (auto &window : _uiWindows)
        {
            window->draw(engine);
        }
    }
} // namespace space
