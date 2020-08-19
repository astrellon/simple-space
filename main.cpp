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
    engine.uiManager().initDefaultWindows();

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

    const space::CharacterDefinition *gregCharDef;
    definitionManager.tryGet("GREG_CHAR", &gregCharDef);

    const space::PlanetSurfaceDefinition *planetGrassySurfaceDef;
    definitionManager.tryGet("PLANET_GRASSY_1", &planetGrassySurfaceDef);

    const space::PlanetSurfaceDefinition *planetSandySurfaceDef;
    definitionManager.tryGet("PLANET_SANDY_1", &planetSandySurfaceDef);

    const space::Dialogue *diag1;
    definitionManager.tryGet("DIAG_1", &diag1);

    const space::PlaceableItemDefinition *chairDef;
    definitionManager.tryGet("CHAIR_1", &chairDef);

    const space::PlaceableItemDefinition *teleporterDef;
    definitionManager.tryGet("TELEPORTER_1", &teleporterDef);

    const space::PlaceableItemDefinition *treeDef;
    definitionManager.tryGet("TREE_1", &treeDef);

    auto gameSession = engine.startGameSession();

    auto planetGrassySurface = gameSession->createPlanetSurface(*planetGrassySurfaceDef);
    auto planetSandySurface = gameSession->createPlanetSurface(*planetSandySurfaceDef);

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

    ship->walkableArea().addCharacter(character);

    auto &player = gameSession->playerController();
    player.controllingCharacter(character);
    player.controllingShip(ship);
    player.controlling(space::ControlShip);

    auto chair = gameSession->createItem<space::Chair>(0, *chairDef);
    player.inventory().addItem(chair);

    chair = gameSession->createItem<space::Chair>(5, *chairDef);
    ship2->walkableArea().addPlaceable(chair, sf::Vector2f(0, -10));

    auto tree = gameSession->createItem<space::PlaceableItem>(10, *treeDef);
    planetGrassySurface->walkableArea().addPlaceable(tree, sf::Vector2f(100, 50));

    auto teleporter = gameSession->createItem<space::Teleporter>(1, *teleporterDef);
    player.inventory().addItem(teleporter);

    teleporter = gameSession->createItem<space::Teleporter>(2, *teleporterDef);
    ship2->walkableArea().addPlaceable(teleporter, sf::Vector2f(0, 0));

    teleporter = gameSession->createItem<space::Teleporter>(3, *teleporterDef);
    planetGrassySurface->walkableArea().addPlaceable(teleporter, sf::Vector2f(50, 50));

    teleporter = gameSession->createItem<space::Teleporter>(4, *teleporterDef);
    planetSandySurface->walkableArea().addPlaceable(teleporter, sf::Vector2f(50, 50));

    gameSession->activeStarSystem(starSystem);
    // gameSession->activePlanetSurface(planetSurface);

    //engine.camera().followingId(character->id);
    // gameSession->setPlayerControllingCharacter();
    gameSession->setPlayerControllingShip(ship);

    auto npc = gameSession->createNpcController();
    auto npcCharacter = gameSession->createObject<space::Character>("GREG", *gregCharDef);
    ship2->walkableArea().addCharacter(npcCharacter);
    npc->controllingCharacter(npcCharacter);
    npc->dialogue(diag1);

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
            gameSession->activePlanetSurface(planetGrassySurface);
            gameSession->moveCharacter(character, sf::Vector2f(), &planetGrassySurface->walkableArea());
        }
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
