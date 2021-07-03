#pragma once

#include <vector>

#include "../ui_element.hpp"

namespace space
{
    class Interactable;
    class UITextElement;
    class UIButton;

    class GameUIInteractable : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUIInteractable() : _interactable(nullptr), _text(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            void interactable(Interactable *interactable);
            Interactable *interactable() const { return _interactable; }
            virtual const char*elementType() const { return "GameUIInteractable"; }

        private:
            // Fields
            GameUIManager *_uiManager;
            Interactable *_interactable;
            UITextElement *_text;
            std::vector<UIButton *> _actionButtons;

            // Methods
    };
} // space