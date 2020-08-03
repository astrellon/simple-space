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
#include "src/serialisers/json/json_serialisers.hpp"
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

#include "earcut.hpp"

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

    engine.initEffects();

    auto &definitionManager = engine.definitionManager();
    definitionManager.loadFolder("data/definitions");

    definitionManager.onPostLoad(engine);

    const space::ShipDefinition *shipDef;
    definitionManager.tryGet("SHIP_1", &shipDef);

    const space::ShipDefinition *shipDef2;
    definitionManager.tryGet("SHIP_2", &shipDef2);

    const space::StarSystemDefinition *starSystemDef;
    definitionManager.tryGet("STAR_SYSTEM_1", &starSystemDef);

    const space::StarSystemDefinition *starSystemDef2;
    definitionManager.tryGet("STAR_SYSTEM_2", &starSystemDef2);

    const space::CharacterDefinition *playerCharDef;
    definitionManager.tryGet("PLAYER_CHAR", &playerCharDef);

    const space::PlanetSurfaceDefinition *planetSurfaceDef;
    definitionManager.tryGet("PLANET_GRASSY_1", &planetSurfaceDef);

    auto gameSession = engine.startGameSession();

    auto planetSurface = gameSession->createPlanetSurface(*planetSurfaceDef);

    auto ship = gameSession->createObject<space::Ship>("PLAYER", *shipDef);
    auto ship2 = gameSession->createObject<space::Ship>("OTHER", *shipDef2);
    ship2->transform().position += sf::Vector2f(200, 0);

    auto starSystem = gameSession->createStarSystem(*starSystemDef);
    starSystem->initFromDefinition();
    starSystem->addObject(ship);
    starSystem->addObject(ship2);

    auto starSystem2 = gameSession->createStarSystem(*starSystemDef2);
    starSystem2->initFromDefinition();

    auto character = gameSession->createObject<space::Character>("PLAYER_CHAR", *playerCharDef);

    // planetSurface->walkableArea().addCharacter(character);

    ship->walkableArea().addCharacter(character);

    space::ItemDefinition chairDefinition("CHAIR_1");
    chairDefinition.name = "Chair";
    chairDefinition.texturePath = "data/items/chair.png";
    chairDefinition.onPostLoad(engine);

    space::ItemDefinition teleporterDefinition("TELEPORTER_1");
    teleporterDefinition.name = "Teleporter";
    teleporterDefinition.texturePath = "data/items/teleporter_still.png";
    teleporterDefinition.onPostLoad(engine);

    auto &player = gameSession->playerController();
    player.controllingCharacter(character);
    player.controllingShip(ship);
    player.controlling(space::ControlShip);

    auto chair = gameSession->createItem<space::Chair>(0, chairDefinition);
    player.inventory().addItem(chair);

    auto teleporter = gameSession->createItem<space::Teleporter>(1, teleporterDefinition);
    player.inventory().addItem(teleporter);

    teleporter = gameSession->createItem<space::Teleporter>(2, teleporterDefinition);
    ship2->walkableArea().addPlaceable(teleporter, sf::Vector2f(0, 0));

    teleporter = gameSession->createItem<space::Teleporter>(3, teleporterDefinition);
    planetSurface->walkableArea().addPlaceable(teleporter, sf::Vector2f(50, 50));

    gameSession->activeStarSystem(starSystem);
    // gameSession->activePlanetSurface(planetSurface);

    //engine.camera().followingId(character->id);
    // gameSession->setPlayerControllingCharacter();
    gameSession->setPlayerControllingShip(ship);

    // auto transition = std::make_unique<space::Transition>(engine.timeSinceStart(), sf::Time::Zero);

    // auto &transFrom = transition->fromData;
    // transFrom.starSystem = starSystem;
    // transFrom.followId = ship->id;

    // auto &transTo = transition->toData;
    // transTo.planetSurface = planetSurface;
    // transTo.cameraScale = 1.0f / space::Utils::getInsideScale();

    // gameSession->setTransition(transition);

    while (window.isOpen())
    {
        engine.processEvents();
        engine.preUpdate();

        if (space::Keyboard::isKeyDown(sf::Keyboard::Num1))
        {
            gameSession->activeStarSystem(starSystem);
            starSystem->addObject(ship);
        }
        if (space::Keyboard::isKeyDown(sf::Keyboard::Num2))
        {
            gameSession->activeStarSystem(starSystem2);
            starSystem2->addObject(ship);
        }
        if (space::Keyboard::isKeyDown(sf::Keyboard::Num3))
        {
            gameSession->activePlanetSurface(planetSurface);
            gameSession->moveCharacter(character, sf::Vector2f(), &planetSurface->walkableArea());
        }
        if (space::Keyboard::isKeyDown(sf::Keyboard::P))
        {
            space::DrawDebug::slow = !space::DrawDebug::slow;
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
