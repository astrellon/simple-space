#pragma once

#include <vector>
#include <observe/event.h>

namespace space
{
    class Interactable;

    class InteractableList
    {
        public:
            typedef observe::Event<Interactable *> InteractableHandler;

            // Fields
            InteractableHandler onAddInteractable;
            InteractableHandler onRemoveInteractable;

            // Constructor

            // Methods
            void addInteractable(Interactable *interactable);
            void removeInteractable(Interactable *interactable);
            void clear();

            const std::vector<Interactable *> &interactables() const { return _interactables; }

        private:
            // Fields
            std::vector<Interactable *> _interactables;

            // Methods
    };
} // space