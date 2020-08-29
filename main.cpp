#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>

#include "src/engine.hpp"
#include "src/resource_manager.hpp"
#include "src/tiles.hpp"
#include "src/game_session.hpp"
#include "src/utils.hpp"
#include "src/definitions/ship_definition.hpp"
#include "src/definitions/character_definition.hpp"
#include "src/definitions/star_system_definition.hpp"
#include "src/definitions/planet_surface_definition.hpp"
#include "src/definitions/item_definition.hpp"
#include "src/definitions/dialogue.hpp"
#include "src/serialisers/json/json_serialisers_definitions.hpp"
#include "src/serialisers/json/json_serialisers_game.hpp"
#include "src/serialisers/json/json.hpp"
#include "src/definition_manager.hpp"
#include "src/game/ship.hpp"
#include "src/game/character.hpp"
#include "src/game/planet.hpp"
#include "src/game/star_system.hpp"
#include "src/game/planet_surface.hpp"
#include "src/game/items/chair.hpp"
#include "src/game/items/teleporter.hpp"
#include "src/keyboard.hpp"
#include "src/physics/polygon_collider.hpp"
#include "src/effects/transition.hpp"
#include "src/debug/draw_debug.hpp"
#include "src/controllers/npc_controller.hpp"
#include "src/ui/ui_manager.hpp"
#include "src/effects/portal_effect.hpp"


void operator delete(void *ptr, size_t size)
{
    space::DrawDebug::totalMemoryAllocated -= size;
    space::DrawDebug::freedThisFrame += size;

    free(ptr);
}

void *operator new(size_t size)
{
    space::DrawDebug::totalMemoryAllocated += size;
    space::DrawDebug::allocatedThisFrame += size;

    return malloc(size);
}

#include "earcut.hpp"

#include <tmxlite/Map.hpp>

int main()
{
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Space", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(120);

    glewInit();

    space::Engine engine(&window);
    engine.spriteScale(2.0f);

    space::ResourceManager &resourceManager = engine.resourceManager();

    resourceManager.preloadShader("stars", "data/shaders/stars.vert", "data/shaders/stars.frag");
    resourceManager.preloadShader("planet", "data/shaders/planet.vert", "data/shaders/planet.frag");
    resourceManager.preloadTextures("data/textures");
    resourceManager.preloadFonts("data/fonts");
    resourceManager.preloadMaps("data/maps");

    engine.initEffects();
    engine.uiManager().initDefaultWindows();

    auto &definitionManager = engine.definitionManager();
    definitionManager.loadFolder("data/definitions");

    definitionManager.onPostLoad(engine);

    std::ifstream startingGameFile("data/startingGame.json");
    nlohmann::json startingGameJson;
    startingGameFile >> startingGameJson;

    auto gameSessionTemp = space::fromJsonGameSession(engine, startingGameJson);
    auto gameSession = gameSessionTemp.get();
    engine.currentSession(std::move(gameSessionTemp));

    auto portalEffect = gameSession->createObject<space::PortalEffect>("PORTAL", 1000);
    space::StarSystem *starSystem1;
    gameSession->tryGetStarSystem("STAR_SYSTEM_1", &starSystem1);

    starSystem1->addObject(portalEffect);

    while (window.isOpen())
    {
        engine.processEvents();
        engine.preUpdate();

        if (space::Keyboard::isKeyDown(sf::Keyboard::P))
        {
            space::DrawDebug::showPolygons = !space::DrawDebug::showPolygons;
        }

        engine.update();
        engine.draw();

        if (space::DrawDebug::slow)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    engine.shutdown();

    return 0;
}
