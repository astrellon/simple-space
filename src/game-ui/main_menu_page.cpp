#include "main_menu_page.hpp"

#include "./main-menu/main_menu_panel.hpp"
#include "./game_ui_manager.hpp"

namespace space
{
    MainMenuPage::MainMenuPage(): _mainMenuPanel(nullptr)
    {

    }

    void MainMenuPage::init(GameUIManager &uiManager)
    {
        _mainMenuPanel = uiManager.createElement<MainMenuPanel>();
        addChild(_mainMenuPanel);

        alignItems(YGAlignCenter);
        justifyContent(YGJustifyCenter);
        widthPercent(100);
        heightPercent(100);
        padding(20, 20, 20, 20);
    }
} // space