#include "inventory.hpp"

#include <algorithm>

namespace space
{
    void Inventory::addItem(Item *item)
    {
        _items.push_back(item);
        onAddItem.emit(item);
    }

    void Inventory::removeItem(Item *item)
    {
        auto find = std::find(_items.begin(), _items.end(), item);
        if (find != _items.end())
        {
            _items.erase(find);
            onRemoveItem.emit(item);
        }
    }
} // namespace space
