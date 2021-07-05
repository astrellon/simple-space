#include "game_scene_manager.hpp"

#include <fstream>

#include "main_menu_scene.hpp"
#include "game_session.hpp"
#include "engine.hpp"
#include "game-ui/game_ui_manager.hpp"
#include "game-ui/ui_element.hpp"
#include "game-ui/main_menu_page.hpp"
#include "game-ui/in_game_ui_page.hpp"

#include "serialisers/json/json.hpp"
#include "serialisers/json/json_serialisers_game.hpp"

namespace space
{
    void GameSceneManager::switchToMainMenu(Engine &engine)
    {
        engine.gameScene(std::make_unique<space::MainMenuScene>(engine));
        engine.gameUIManager().currentPage(engine.gameUIManager().mainMenuPage());
    }

    void GameSceneManager::startNewGame(Engine &engine)
    {
        std::ifstream startingGameFile("data/startingGame.json");
        nlohmann::json startingGameJson;
        startingGameFile >> startingGameJson;

        engine.gameScene(space::fromJsonGameSession(engine, startingGameJson));
        engine.gameUIManager().currentPage(engine.gameUIManager().inGameUIPage());
    }
} // space