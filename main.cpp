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
#include "src/serialisers/json/json_serialisers.hpp"
#include "src/serialisers/json/json.hpp"
#include "src/definition_manager.hpp"
#include "src/game/ship.hpp"
#include "src/game/character.hpp"
#include "src/game/planet.hpp"
#include "src/game/star_system.hpp"
#include "src/keyboard.hpp"

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

    using Point = std::array<double, 2>;
    std::vector<std::vector<Point>> polygon;

    // Fill polygon structure with actual data. Any winding order works.
    // The first polyline defines the main polygon.
    polygon.push_back({{100, 0}, {100, 100}, {0, 100}, {0, 0}});
    // Following polylines define holes.
    polygon.push_back({{75, 25}, {75, 75}, {25, 75}, {25, 25}});

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    std::vector<sf::VertexArray *> polygons;
    for (auto i = 0; i < indices.size(); i += 3)
    {
        auto v = new sf::VertexArray(sf::TrianglesFan, 3);
        for (auto j = 0; j < 3; j++)
        {
            auto index = indices[i + j];
            Point p;
            if (index >= polygon[0].size())
            {
                p = polygon[1][index - polygon[0].size()];
            }
            else
            {
                p = polygon[0][index];
            }

            (*v)[j].position = sf::Vector2f(p[0], p[1]);
            (*v)[j].color = space::Utils::hsv((float)i / (float)indices.size() * 360, 1, 1);
            (*v)[j].color.a = 120;
        }

        polygons.push_back(v);
    }

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

    const space::StarSystemDefinition *starSystemDef2;
    definitionManager.tryGet("STAR_SYSTEM_2", &starSystemDef2);

    const space::CharacterDefinition *playerCharDef;
    definitionManager.tryGet("PLAYER_CHAR", &playerCharDef);

    auto gameSession = engine.startGameSession();

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
    //character->transform().scale = 0.2f;
    //starSystem->addObject(character);

    ship->walkableArea().addCharacter(character);

    gameSession->playerController().controllingCharacter(character);
    gameSession->playerController().controllingShip(ship);
    gameSession->playerController().controlling(space::ControlShip);

    gameSession->activeStarSystem(starSystem);

    engine.camera().followingId(character->id);

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

        engine.update();
        engine.draw();

        sf::Transform trans;
        trans.translate(50, 50);
        for (auto p : polygons)
        {
            window.draw(*p, trans);
        }

        window.display();
    }

    return 0;
}
