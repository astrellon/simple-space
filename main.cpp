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
#include "src/definitions/star_system_definition.hpp"
#include "src/serialisers/json/json_serialisers.hpp"
#include "src/serialisers/json/json.hpp"
#include "src/definition_manager.hpp"
#include "src/game/ship.hpp"
#include "src/game/planet.hpp"
#include "src/game/star_system.hpp"
#include "src/keyboard.hpp"

#include <tmxlite/Map.hpp>

int main()
{
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(1280, 800), "Space", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(120);

    glewInit();

    space::Engine engine(window);
    engine.spriteScale(2.0f);

    space::ResourceManager &resourceManager = engine.resourceManager();

    resourceManager.loadEmbedded();
    resourceManager.preloadShader("stars", "data/shaders/stars.vert", "data/shaders/stars.frag");
    resourceManager.preloadShader("planet", "data/shaders/planet.vert", "data/shaders/planet.frag");
    resourceManager.preloadTextures("data/textures");
    resourceManager.preloadFonts("data/fonts");
    resourceManager.preloadMaps("data/maps");

    engine.initBackground();

    auto &definitionManager = engine.definitionManager();
    definitionManager.loadFolder("data/definitions");

    definitionManager.onPostLoad(engine);

    const space::ShipDefinition *shipDef;
    definitionManager.tryGet("SHIP_1", &shipDef);

    const space::ShipDefinition *shipDef2;
    definitionManager.tryGet("SHIP_2", &shipDef2);

    const space::StarSystemDefinition *starSystemDef;
    definitionManager.tryGet("STAR_SYSTEM_1", &starSystemDef);

    auto spriteSize = static_cast<uint>(engine.spriteSize());
    auto spriteScale = engine.spriteScale();

    auto gameSession = engine.startGameSession();

    auto ship = gameSession->createObject<space::Ship>("PLAYER", *shipDef);
    auto ship2 = gameSession->createObject<space::Ship>("OTHER", *shipDef2);
    ship2->transform().position += sf::Vector2f(200, 0);

    auto starSystem = gameSession->createStarSystem(*starSystemDef);
    starSystem->initFromDefinition();
    starSystem->addObject(ship);
    starSystem->addObject(ship2);

    gameSession->activeStarSystem(starSystem);

    engine.camera().setFollowing(ship->id);

    while (window.isOpen())
    {
        engine.processEvents();
        engine.preUpdate();

        sf::Vector2f moveInput;
        float rotateInput = 0;
        if (space::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            moveInput.x -= 1;
        }
        if (space::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            moveInput.x += 1;
        }
        if (space::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            moveInput.y -= 1;
        }
        if (space::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            moveInput.y += 1;
        }
        if (space::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            rotateInput -= 1;
        }
        if (space::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            rotateInput += 1;
        }

        ship->moveInput = moveInput;
        ship->rotateInput = rotateInput;

        engine.update();
        engine.draw();
    }

    return 0;
}
