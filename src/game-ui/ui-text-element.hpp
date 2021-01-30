#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "ui-element.hpp"

namespace space
{
    class UITextElement : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            void text(const std::string &value) { _textElement.setString(value); }
            const std::string &text() const { return _textElement.getString(); }

            sf::Text &textElement() { return _textElement; }
            const sf::Text &textElement() const { return _textElement; }

            virtual ElementType elementType() const { return ElementType::Text; }

        protected:
            // Methods
            virtual void drawSelf(Engine &engine, RenderCamera &target);

        private:
            // Fields
            sf::Text _textElement;

            // Methods
    };
} // space