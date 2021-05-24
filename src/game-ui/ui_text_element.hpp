#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "ui_element.hpp"

namespace space
{
    class GameUIManager;

    class UITextElement : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual void init(GameUIManager &uiManager);

            void text(const std::string &value) { _textElement.setString(value); }
            std::string text() const { return _textElement.getString(); }

            void font(const sf::Font *font) { if (font) { _textElement.setFont(*font); } }
            void font(const sf::Font &font) { _textElement.setFont(font); }
            const sf::Font *font() const { return _textElement.getFont(); }

            sf::Text &textElement() { return _textElement; }
            const sf::Text &textElement() const { return _textElement; }

            virtual const char *elementType() const { return "Text"; }

        protected:
            // Methods
            virtual void drawSelf(Engine &engine, RenderCamera &target);

        private:
            // Fields
            sf::Text _textElement;

            // Methods
    };
} // space