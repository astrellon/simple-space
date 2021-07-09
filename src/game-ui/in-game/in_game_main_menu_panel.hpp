#pragma once

#include "../ui_element.hpp"

namespace space
{
    class UIPanel;
    class UIButton;

    class InGameMainMenuPanel : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual void init(GameUIManager &uiManager);

            virtual const char *elementType() const { return "InGameMainMenuPanel"; }

            virtual void preUpdate(Engine &engine, sf::Time dt);

        private:
            // Fields
            UIPanel *_panel = nullptr;
            UIButton *_resumeGameButton = nullptr;
            UIButton *_saveGameButton = nullptr;
            UIButton *_loadGameButton = nullptr;
            UIButton *_backToMainMenuButton = nullptr;
            UIButton *_exitButton = nullptr;

            // Methods
    };
} // space