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

    json toJson(const GameSession &input);
    std::unique_ptr<GameSession> fromGameSession(Engine &engine, const json &j);

    json toJsonBase(const SpaceObject &input);
    json toJson(const SpaceObject &input);
    json toJson(const Character &input);
    json toJson(const Ship &input);
    json toJson(const Planet &input);
    json toJson(const PlacedItem &input);

    json toJson(const WalkableArea &input);
    json toJson(const Inventory &input);

    json toJsonBase(const CharacterController &input);
    json toJson(const CharacterController &input);
    json toJson(const NpcController &input);
    json toJson(const PlayerController &input);

    json toJsonBase(const Item &item);
    json toJson(const Item &item);
    json toJson(const PlaceableItem &item);

    json toJson(const SpaceTransform &input);
}