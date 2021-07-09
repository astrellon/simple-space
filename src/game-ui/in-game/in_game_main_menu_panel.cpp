#include "in_game_main_menu_panel.hpp"

#include "../ui_text_element.hpp"
#include "../ui_button.hpp"
#include "../ui_panel.hpp"
#include "../game_ui_manager.hpp"

#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../game_scene_manager.hpp"

namespace space
{
    void InGameMainMenuPanel::init(GameUIManager &uiManager)
    {
        auto &engine = uiManager.engine();
        width(200);
        flexShrink(1.0f);
        positionType(YGPositionTypeAbsolute);

        _panel = uiManager.createElement<UIPanel>();
        addChild(_panel);
        _panel->flexShrink(1.0f);

        auto heading = uiManager.createElement<UITextElement>();
        heading->text("Main Menu");
        _panel->bodyContainer()->addChild(heading);

        _resumeGameButton = uiManager.createElement<UIButton>();
        _resumeGameButton->text("Resume Game");
        _resumeGameButton->widthPercent(100);
        _resumeGameButton->onClick([&engine, this](const sf::Event &e)
        {
            this->visible(false);
            return UIEventResult::Triggered;
        });
        _panel->bodyContainer()->addChild(_resumeGameButton);

        _saveGameButton = uiManager.createElement<UIButton>();
        _saveGameButton->text("Save Game");
        _saveGameButton->widthPercent(100);
        _panel->bodyContainer()->addChild(_saveGameButton);

        _loadGameButton = uiManager.createElement<UIButton>();
        _loadGameButton->text("Load Game");
        _loadGameButton->widthPercent(100);
        _panel->bodyContainer()->addChild(_loadGameButton);

        _backToMainMenuButton = uiManager.createElement<UIButton>();
        _backToMainMenuButton->text("Back To Main Menu");
        _backToMainMenuButton->widthPercent(100);
        _backToMainMenuButton->onClick([&engine](const sf::Event &e)
        {
            GameSceneManager::switchToMainMenu(engine);
            return UIEventResult::Triggered;
        });
        _panel->bodyContainer()->addChild(_backToMainMenuButton);

        _exitButton = uiManager.createElement<UIButton>();
        _exitButton->text("Exit");
        _exitButton->widthPercent(100);
        _exitButton->onClick([&engine](const sf::Event &e)
        {
            engine.window()->close();
            return UIEventResult::Triggered;
        });
        _panel->bodyContainer()->addChild(_exitButton);
    }

    void InGameMainMenuPanel::preUpdate(Engine &engine, sf::Time dt)
    {
        auto screenSize = engine.renderSize();
        margin(20, 0, 0, screenSize.x / 2 - 100);
    }
} // space