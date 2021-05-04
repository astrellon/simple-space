#pragma once

#include "./game-ui-window.hpp"
#include <observe/observer.h>

namespace space
{
    class Item;
    class Inventory;
    class GameUIInventoryItem;

    class GameUIInventoryWindow : public GameUIWindow
    {
        public:
            // Fields

            // Constructor
            GameUIInventoryWindow() : GameUIWindow() { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            void inventory(Inventory *inventory);
            Inventory *inventory() { return _inventory; }

            virtual const char*elementType() const { return "GameUIInventory"; }

        private:
            // Fields
            Inventory *_inventory;
            observe::Observer _removeOnAddItem;
            observe::Observer _removeOnRemoveItem;
            GameUIManager *_uiManager;

            std::vector<GameUIInventoryItem *> _itemUIs;

            // Methods
            void addItem(Item *item);
            void removeItem(Item *item);
    };
} // space