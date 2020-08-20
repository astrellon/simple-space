#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "json.hpp"

using nlohmann::json;

namespace space
{
    class GameSession;
    class Engine;
    class SpaceObject;
    class Character;
    class Ship;
    class Planet;
    class PlacedItem;
    class WalkableArea;
    class SpaceTransform;
    class CharacterController;
    class NpcController;
    class PlayerController;
    class Inventory;
    class Item;
    class PlaceableItem;
    class DefinitionManager;
    class StarSystem;
    class PlanetSurface;

    json toJson(const GameSession &input);
    std::unique_ptr<GameSession> fromGameSession(Engine &engine, const json &j);

    json toJsonBase(const SpaceObject &input);
    json toJson(const SpaceObject &input);
    std::unique_ptr<SpaceObject> fromJsonSpaceObject(const json &j, GameSession &session);

    json toJson(const Character &input);
    std::unique_ptr<Character> fromJsonCharacter(const json &j, GameSession &session);

    json toJson(const Ship &input);
    std::unique_ptr<Ship> fromJsonShip(const json &j, GameSession &session);

    json toJson(const Planet &input);
    std::unique_ptr<Planet> fromJsonPlanet(const json &j, GameSession &session);

    json toJson(const PlacedItem &input);
    bool addFromJsonPlacedItem(const json &j, GameSession &session, WalkableArea &area);

    json toJson(const WalkableArea &input);
    std::unique_ptr<WalkableArea> fromJsonWalkableArea(const json &j, GameSession &session);

    json toJson(const StarSystem &input);
    json toJson(const PlanetSurface &input);

    json toJson(const Inventory &input);

    json toJsonBase(const CharacterController &input);
    json toJson(const CharacterController &input);
    json toJson(const NpcController &input);
    json toJson(const PlayerController &input);

    json toJsonBase(const Item &item);
    json toJson(const Item &item);
    json toJson(const PlaceableItem &item);

}