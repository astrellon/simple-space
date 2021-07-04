#pragma once

#include "../ui_element.hpp"

namespace space
{
    class UIPanel;
    class UIButton;

    class MainMenuPanel : public UIElement
    {
        public:
            // Fields

            // Constructor
            MainMenuPanel();
            virtual ~MainMenuPanel() { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            virtual const char *elementType() const { return "MainMenuPanel"; }

        private:
            // Fields
            UIPanel *_panel;
            UIButton *_startGameButton;
            UIButton *_loadGameButton;
            UIButton *_editorButton;
            UIButton *_exitButton;

            // Methods
    };
} // space