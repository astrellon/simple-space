#include "interactable_list.hpp"

#include <algorithm>

namespace space
{
    void InteractableList::addInteractable(Interactable *interactable)
    {
        auto find = std::find(_interactables.begin(), _interactables.end(), interactable);
        if (find == _interactables.end())
        {
            _interactables.push_back(interactable);
            onAddInteractable.emit(interactable);
        }
    }

    void InteractableList::removeInteractable(Interactable *interactable)
    {
        auto find = std::find(_interactables.begin(), _interactables.end(), interactable);
        if (find != _interactables.end())
        {
            _interactables.erase(find);
            onRemoveInteractable.emit(interactable);
        }
    }

    void InteractableList::clear()
    {
        for (auto interactable : _interactables)
        {
            onRemoveInteractable.emit(interactable);
        }

        _interactables.clear();
    }
} // space