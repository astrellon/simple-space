#include "main_menu_panel.hpp"

#include "../ui_text_element.hpp"
#include "../ui_button.hpp"
#include "../ui_panel.hpp"
#include "../game_ui_manager.hpp"

#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../game_scene_manager.hpp"

namespace space
{
    MainMenuPanel::MainMenuPanel() : _panel(nullptr),
        _startGameButton(nullptr),
        _loadGameButton(nullptr),
        _editorButton(nullptr),
        _exitButton(nullptr)
    {

    }

    void MainMenuPanel::init(GameUIManager &uiManager)
    {
        auto &engine = uiManager.engine();
        width(200);
        flexShrink(1.0f);

        _panel = uiManager.createElement<UIPanel>();
        addChild(_panel);
        _panel->flexShrink(1.0f);

        auto heading = uiManager.createElement<UITextElement>();
        heading->text("Main Menu");
        _panel->bodyContainer()->addChild(heading);

        _startGameButton = uiManager.createElement<UIButton>();
        _startGameButton->text("Start Game");
        _startGameButton->widthPercent(100);
        _startGameButton->onClick([&engine](const sf::Event &e)
        {
            GameSceneManager::startNewGame(engine);
            return UIEventResult::Triggered;
        });
        _panel->bodyContainer()->addChild(_startGameButton);

        _loadGameButton = uiManager.createElement<UIButton>();
        _loadGameButton->text("Load Game");
        _loadGameButton->widthPercent(100);
        _panel->bodyContainer()->addChild(_loadGameButton);

        _editorButton = uiManager.createElement<UIButton>();
        _editorButton->text("Start Editor");
        _editorButton->widthPercent(100);
        _panel->bodyContainer()->addChild(_editorButton);

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
} // space