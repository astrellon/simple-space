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

            // Methods
            virtual void init(GameUIManager &uiManager);

            virtual const char *elementType() const { return "MainMenuPanel"; }

        private:
            // Fields
            UIPanel *_panel = nullptr;
            UIButton *_startGameButton = nullptr;
            UIButton *_loadGameButton = nullptr;
            UIButton *_editorButton = nullptr;
            UIButton *_exitButton = nullptr;

            // Methods
    };
} // space