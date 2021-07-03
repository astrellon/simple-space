#include "game_ui_inventory_window.hpp"

#include "./game_ui_inventory_item.hpp"

#include "../game_ui_manager.hpp"
#include "../ui_text_element.hpp"
#include "../ui_button.hpp"

#include "../../game/items/item.hpp"
#include "../../game/inventory.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../keyboard.hpp"

namespace space
{
    void GameUIInventoryWindow::init(GameUIManager &uiManager)
    {
        GameUIWindow::init(uiManager);
        _uiManager = &uiManager;

        width(200);
        heightAuto();
        flexShrink(1.0f);
        margin(16, 16, 16, 16);

        header()->text("Inventory");
    }

    void GameUIInventoryWindow::draw(Engine &engine, RenderCamera &target)
    {
        if (_inventory == nullptr)
        {
            return;
        }

        UIElement::draw(engine, target);
    }

    void GameUIInventoryWindow::inventory(Inventory *inventory)
    {
        if (inventory == _inventory)
        {
            return;
        }

        for (auto itemUI : _itemUIs)
        {
            bodyContainer()->removeChild(itemUI);
            _uiManager->removeElement(itemUI);
        }
        _itemUIs.clear();

        _removeOnAddItem.reset();
        _removeOnRemoveItem.reset();

        _inventory = inventory;

        if (inventory)
        {
            for (auto item : inventory->items())
            {
                addItem(item);
            }

            _removeOnAddItem = inventory->onAddItem.createObserver([this](Item *item)
            {
                std::cout << "Added item" << std::endl;
                this->addItem(item);
            });
            _removeOnRemoveItem = inventory->onRemoveItem.createObserver([this](Item *item)
            {
                std::cout << "Removed item" << std::endl;
                this->removeItem(item);
            });
        }
    }

    void GameUIInventoryWindow::preUpdate(Engine &engine, sf::Time dt)
    {
        if (destroyed())
        {
            return;
        }

        if (engine.currentSession())
        {
            if (Keyboard::isKeyUp(sf::Keyboard::I))
            {
                if (inventory() == nullptr)
                {
                    auto &inv = engine.currentSession()->playerController().inventory();
                    inventory(&inv);
                }
                else
                {
                    inventory(nullptr);
                }
            }
        }
        else
        {
            inventory(nullptr);
        }

        UIElement::preUpdate(engine, dt);
    }

    void GameUIInventoryWindow::addItem(Item *item)
    {
        auto itemUI = _uiManager->createElement<GameUIInventoryItem>();
        itemUI->item(item);

        bodyContainer()->addChild(itemUI);
        _itemUIs.push_back(itemUI);
    }

    void GameUIInventoryWindow::removeItem(Item *item)
    {
        for (auto iter = _itemUIs.begin(); iter != _itemUIs.end(); ++iter)
        {
            auto itemUI = *iter;
            if (itemUI->item() == item)
            {
                _itemUIs.erase(iter);
                bodyContainer()->removeChild(itemUI);
                _uiManager->removeElement(itemUI);
                break;
            }
        }
    }
} // space