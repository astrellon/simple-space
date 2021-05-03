#include "game-ui-inventory-window.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-button.hpp"

#include "../game/inventory.hpp"

namespace space
{
    void GameUIInventoryWindow::init(GameUIManager &uiManager)
    {
        GameUIWindow::init(uiManager);

        width(256);
        height(256);

        header()->text("Inventory");

        auto textElement = uiManager.createElement<UITextElement>();
        textElement->text("Hello there!");

        bodyContainer()->addChild(textElement);

        auto button = uiManager.createElement<UIButton>();
        bodyContainer()->addChild(button);
    }

    void GameUIInventoryWindow::inventory(Inventory *inventory)
    {
        _removeOnAddItem.reset();
        _removeOnRemoveItem.reset();

        _inventory = inventory;

        if (inventory)
        {
            inventory->onAddItem.connect([](Item *item)
            {
                std::cout << "Added item" << std::endl;
            });
            inventory->onRemoveItem.connect([](Item *item)
            {
                std::cout << "Removed item" << std::endl;
            });
        }
    }
} // space