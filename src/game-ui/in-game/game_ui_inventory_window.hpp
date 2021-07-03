#pragma once

#include "../game_ui_window.hpp"
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
            GameUIInventoryWindow() : GameUIWindow(), _inventory(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);
            virtual void draw(Engine &engine, RenderCamera &target);
            virtual void preUpdate(Engine &engine, sf::Time dt);

            void inventory(Inventory *inventory);
            Inventory *inventory() const { return _inventory; }

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