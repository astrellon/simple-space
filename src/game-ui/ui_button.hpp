#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "ui_element.hpp"

namespace space
{
    class UITextElement;
    class UINineSliceImageElement;

    class UIButton : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual void init(GameUIManager &uiManager);

            UITextElement *textElement() { return _textElement; }
            const UITextElement *textElement() const { return _textElement; }

            virtual const char*elementType() const { return "Button"; }

            void text(const std::string &text);
            std::string text() const;

        private:
            // Fields
            UITextElement *_textElement;
            //sf::RectangleShape _backPanel;
            UINineSliceImageElement *_backPanel;
    };
} // space