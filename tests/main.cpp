#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/engine.hpp"
#include "../src/resource_manager.hpp"
#include "../src/definitions/ship_definition.hpp"
#include "../src/definitions/character_definition.hpp"

using namespace space;

TEST_CASE("testing serialise definitions")
{
    Engine engine(nullptr);
    engine.resourceManager().preloadTextures("data/textures");
    engine.definitionManager().loadFolder("data/definitions");

    engine.definitionManager().onPostLoad(engine);

    const ShipDefinition *shipDefinition;
    REQUIRE(engine.definitionManager().tryGet("SHIP_1", &shipDefinition));

    REQUIRE_EQ(shipDefinition->name, "Player Ship");
    REQUIRE_EQ(shipDefinition->maxRotation, 180.0f);
    REQUIRE_EQ(shipDefinition->maxSpeed, 200.0f);
    REQUIRE_EQ(shipDefinition->turnRate, 180.0f);
    REQUIRE_EQ(shipDefinition->acceleration, 250.0f);
    REQUIRE_EQ(shipDefinition->texture->getSize(), sf::Vector2u(64, 64));
    REQUIRE_EQ(shipDefinition->interiorTexture->getSize(), sf::Vector2u(256, 256));
    REQUIRE_EQ(shipDefinition->engineGlowTexture->getSize(), sf::Vector2u(15, 15));

    REQUIRE_EQ(shipDefinition->enginePlacements.size(), 2);
    REQUIRE_EQ(shipDefinition->enginePlacements[0], sf::Vector2f(-18, 14));
    REQUIRE_EQ(shipDefinition->enginePlacements[1], sf::Vector2f(18, 14));

    REQUIRE_EQ(shipDefinition->interiorPolygon.size(), 6);
    REQUIRE_EQ(shipDefinition->interiorPolygon[0], sf::Vector2f(-4, -12));
    REQUIRE_EQ(shipDefinition->interiorPolygon[1], sf::Vector2f(4, -12));
    REQUIRE_EQ(shipDefinition->interiorPolygon[2], sf::Vector2f(10, 3));
    REQUIRE_EQ(shipDefinition->interiorPolygon[3], sf::Vector2f(3, 12));
    REQUIRE_EQ(shipDefinition->interiorPolygon[4], sf::Vector2f(-3, 12));
    REQUIRE_EQ(shipDefinition->interiorPolygon[5], sf::Vector2f(-10, 3));

    const CharacterDefinition *charDefinition;
    REQUIRE(engine.definitionManager().tryGet("PLAYER_CHAR", &charDefinition));

    REQUIRE_EQ(charDefinition->name, "Player");
    REQUIRE_EQ(charDefinition->spriteSize, 32);
    REQUIRE_EQ(charDefinition->speed, 50);
    REQUIRE_EQ(charDefinition->texture->getSize(), sf::Vector2u(128, 32));
}