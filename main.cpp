#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>

#include "src/engine.hpp"
#include "src/font_manager.hpp"
#include "src/tiles.hpp"
#include "src/game_session.hpp"
#include "src/particle_system.hpp"
#include "src/utils.hpp"
#include "src/definitions/ship_definition.hpp"
#include "src/serialisers/json/json_serialisers.hpp"
#include "src/serialisers/json/json.hpp"
#include "src/definition_manager.hpp"
#include "src/game/ship.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Starter");
    window.setVerticalSyncEnabled(true);

    space::Engine engine(window);
    engine.spriteScale(4.0f);

    auto &textureManager = engine.textureManager();
    textureManager.loadFolder("data/textures");

    engine.fontManager().loadFolder("data/fonts");

    auto &definitionManager = engine.definitionManager();
    definitionManager.loadFolder("data/definitions");

    definitionManager.onPostLoad(engine);

    const space::ShipDefinition *shipDef;
    if (definitionManager.tryGet("SHIP_1", &shipDef))
    {
        std::cout << "Ship Def: " << shipDef->name << std::endl;
    }

    auto sansFont = engine.fontManager().font("LiberationSans-Regular");

    auto spriteSize = static_cast<uint>(engine.spriteSize());
    auto spriteScale = engine.spriteScale();

    auto gameSession = engine.startGameSession();
    auto ship = gameSession->createShip("SHIP", *shipDef);

    engine.camera().setFollowing(ship->id);

    sf::Text text("hello", *sansFont);
    text.setCharacterSize(72);

    while (window.isOpen())
    {
        engine.processEvents();
        engine.preUpdate();
        engine.update();
        engine.draw();

        // window.draw(text);
        // window.display();
    }

    return 0;
}
