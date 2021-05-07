#pragma once

#include <vector>
#include <observe/event.h>

namespace space
{
    class Item;

    class Inventory
    {
        public:
            typedef observe::Event<Item *> ItemHandler;

            // Fields
            ItemHandler onAddItem;
            ItemHandler onRemoveItem;

            // Constructor

            // Methods
            void addItem(Item *item);
            void removeItem(Item *item);

            const std::vector<Item *> &items() const { return _items; }

        private:
            // Fields
            std::vector<Item *> _items;

            // Methods
    };
} // space