#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "ui-element.hpp"

namespace space
{
    class UITextElement;

    class UIButton : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual void init(GameUIManager &uiManager);

            UITextElement *textElement() { return _textElement; }
            const UITextElement *textElement() const { return _textElement; }

            virtual ElementType elementType() const { return ElementType::Button; }

        protected:
            // Methods
            virtual void drawSelf(Engine &engine, RenderCamera &target);

        private:
            // Fields
            UITextElement *_textElement;
            sf::RectangleShape _backPanel;
    };
} // space