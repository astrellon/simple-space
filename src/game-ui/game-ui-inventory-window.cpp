#include "game-ui-inventory-window.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-button.hpp"
#include "./game-ui-inventory-item.hpp"

#include "../game/items/item.hpp"
#include "../game/inventory.hpp"

namespace space
{
    void GameUIInventoryWindow::init(GameUIManager &uiManager)
    {
        GameUIWindow::init(uiManager);
        _uiManager = &uiManager;

        margin(16, 0, 0, 16);
        width(200);
        height(200);

        header()->text("Inventory");
    }

    void GameUIInventoryWindow::inventory(Inventory *inventory)
    {
        _removeOnAddItem.reset();
        _removeOnRemoveItem.reset();

        _inventory = inventory;

        if (inventory)
        {
            for (auto item : inventory->items())
            {
                addItem(item);
            }

            inventory->onAddItem.connect([this](Item *item)
            {
                std::cout << "Added item" << std::endl;
                this->addItem(item);
            });
            inventory->onRemoveItem.connect([this](Item *item)
            {
                std::cout << "Removed item" << std::endl;
                this->removeItem(item);
            });
        }
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