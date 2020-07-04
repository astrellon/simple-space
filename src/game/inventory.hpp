#pragma once

#include <vector>

namespace space
{
    class Item;

    class Inventory
    {
        public:
            // Fields

            // Constructor

            // Methods
            void addItem(Item *item);
            void removeItem(Item *item);

            const std::vector<Item *> items() const { return _items; }

        private:
            // Fields
            std::vector<Item *> _items;

            // Methods
    };
} // space