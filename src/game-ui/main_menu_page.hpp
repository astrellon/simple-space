#pragma once

#include <string>

#include "./ui_element.hpp"

namespace space
{
    class UIButton;

    class MainMenuPanel;

    class MainMenuPage : public UIElement
    {
        public:
            // Fields

            // Constructor
            MainMenuPage();
            virtual ~MainMenuPage() { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            virtual const char *elementType() const { return "MainMenuPage"; }

        private:
            // Fields
            MainMenuPanel *_mainMenuPanel;

            // Methods
    };
} // space