#pragma once

#include "../ui_element.hpp"

namespace space
{
    class Item;
    class UITextElement;
    class UIButton;

    class GameUIInventoryItem : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUIInventoryItem() : _item(nullptr), _text(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            void item(Item *item);
            Item *item() const { return _item; }

            virtual const char*elementType() const { return "GameUIInventoryItem"; }

        private:
            // Fields
            Item *_item;
            UITextElement *_text;
            UIButton *_dropButton;

            // Methods
    };
} // space