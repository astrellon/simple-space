#pragma once

#include "../ui_element.hpp"

#include <observe/observer.h>

namespace space
{
    class Interactable;
    class InteractableList;
    class GameUIInteractable;
    class UIPanel;

    class GameUIInteractablesPanel : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUIInteractablesPanel() : UIElement(), _interactables(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);
            virtual void draw(Engine &engine, RenderCamera &target);
            virtual void preUpdate(Engine &engine, sf::Time dt);

            void interactables(const InteractableList *interactables);
            const InteractableList *interactables() const { return _interactables; }

            virtual const char *elementType() const { return "GameUIInteractables"; }

        private:
            // Fields
            const InteractableList *_interactables;
            observe::Observer _removeOnAddInteractable;
            observe::Observer _removeOnRemoveInteractable;
            GameUIManager *_uiManager;

            UIPanel *_panel;

            std::vector<GameUIInteractable *> _interactableUIs;

            // Methods
            void addInteractable(Interactable *item);
            void removeInteractable(Interactable *item);
    };
} // space