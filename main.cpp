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

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Starter");
    window.setVerticalSyncEnabled(true);

    space::Engine engine(window);
    engine.spriteScale(4.0f);

    auto textureManager = engine.textureManager();
    auto shipTexture = textureManager->load_texture("data/textures/ships/ship1.png");

    engine.fontManager()->load_fonts("data/fonts");

    // space::ShipDefinition shipDef;
    // shipDef.id = "SHIP_1";
    // shipDef.name = "Ship Def";

    // nlohmann::json j = shipDef;

    // std::cout << std::setw(4) << j << std::endl;
    space::DefinitionManager defManager;

    const space::ShipDefinition *shipDef = defManager.load<space::ShipDefinition>("data/definitions/ships.json");
    if (shipDef != nullptr)
    {
        std::cout << "Loaded: " << shipDef->name << std::endl;
    }
    else
    {
        std::cout << "Failed to load :(" << std::endl;
    }

    auto sansFont = engine.fontManager()->font("LiberationSans-Regular");

    auto spriteSize = static_cast<uint>(engine.spriteSize());
    auto spriteScale = engine.spriteScale();

    auto gameSession = engine.startGameSession();
    auto ship = gameSession->createShip(shipTexture, 10, 10);

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
