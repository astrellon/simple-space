#include "game-ui-inventory.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"

#include "../game/inventory.hpp"

namespace space
{
    void GameUIInventory::init(GameUIManager &uiManager)
    {
        GameUIWindow::init(uiManager);

        width(256);
        height(256);

        header()->text("Inventory");

        auto textElement = uiManager.createElement<space::UITextElement>();
        textElement->text("Hello there!");

        bodyContainer()->addChild(textElement);
    }

    void GameUIInventory::inventory(Inventory *inventory)
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