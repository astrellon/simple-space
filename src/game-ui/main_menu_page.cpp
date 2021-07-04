#include "main_menu_page.hpp"

#include "./main-menu/main_menu_panel.hpp"
#include "./game_ui_manager.hpp"

namespace space
{
    MainMenuPage::MainMenuPage()
    {

    }

    void MainMenuPage::init(GameUIManager &uiManager)
    {
        _mainMenuPanel = uiManager.createElement<MainMenuPanel>();
        addChild(_mainMenuPanel);

        alignItems(YGAlignCenter);
        justifyContent(YGJustifyCenter);
    }
} // space