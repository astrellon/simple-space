#include "game_scene_manager.hpp"

#include <fstream>

#include "utils.hpp"
#include "main_menu_scene.hpp"
#include "game_session.hpp"
#include "editor_game_session.hpp"
#include "engine.hpp"
#include "game-ui/game_ui_manager.hpp"
#include "game-ui/ui_element.hpp"
#include "game-ui/main_menu_page.hpp"
#include "game-ui/in_game_ui_page.hpp"
#include "ui/ui_manager.hpp"
#include "ui/editor/ui_area_selector.hpp"
#include "ui/editor/ui_selected.hpp"
#include "ui/editor/ui_editor_menu.hpp"

#include "serialisers/json/json.hpp"
#include "serialisers/json/json_serialisers_game.hpp"

#include "definition_manager.hpp"
#include "definitions/placeable_item_definition.hpp"
#include "game/planet_surface.hpp"

namespace space
{
    void addTrees(Engine &engine)
    {
        auto session = engine.currentSession();

        auto &definitionManager = engine.definitionManager();
        const PlaceableItemDefinition *tree1;

        if (!definitionManager.tryGet<space::PlaceableItemDefinition>("TREE_1", &tree1))
        {
            std::cout << "Could not find tree :(\n";
            return;
        }

        PlanetSurface *planetSurface;
        if (!session->tryGetSpaceObject<PlanetSurface>("PLANET_GRASSY_1", planetSurface))
        {
            std::cout << "Could not find planet surface :(\n";
            return;
        }

        auto itemId = 100;
        for (auto i = 0; i < 100000; i++)
        {
            auto newTree = session->createItem<PlaceableItem>(itemId++, *tree1);
            auto step = i / 1000;
            auto offset = step % 2 ? 30 : 0;
            auto y = step * 80;
            auto x = (i % 1000) * 40 + offset;
            planetSurface->area().addPlaceable(*session, newTree, sf::Vector2f(x, y));
        }
    }

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

        // addTrees(engine);
    }

    void GameSceneManager::startEditor(Engine &engine)
    {
        std::ifstream startingGameFile("data/startingGame.json");
        nlohmann::json startingGameJson;
        startingGameFile >> startingGameJson;

        engine.gameScene(space::fromJsonEditorGameSession(engine, startingGameJson));
        engine.gameUIManager().currentPage(nullptr);
        engine.uiManager().uiAreaSelector().show = true;
        engine.uiManager().uiSelected().show = true;
        engine.uiManager().uiEditorMenu().show = true;

        // addTrees(engine);
    }

} // space