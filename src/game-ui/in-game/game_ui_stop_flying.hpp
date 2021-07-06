#pragma once

#include "../ui_element.hpp"

namespace space
{
    class UIButton;

    class GameUIStopFlying : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUIStopFlying() : UIElement(), _button(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);
            virtual void preUpdate(Engine &engine, sf::Time dt);

            virtual const char *elementType() const { return "GameUIStopFlying"; }

        private:
            // Fields
            UIButton *_button;

            // Methods
    };
} // space