#pragma once

#include "./game-ui-window.hpp"
#include <observe/observer.h>

namespace space
{
    class Inventory;

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

        private:
            // Fields
            Inventory *_inventory;
            observe::Observer _removeOnAddItem;
            observe::Observer _removeOnRemoveItem;

            // Methods
    };
} // space