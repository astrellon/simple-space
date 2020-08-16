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

    json toJson(const GameSession &input);
    std::unique_ptr<GameSession> fromGameSession(Engine &engine, const json &j);

    json toJson(const SpaceObject &input);
    json toJson(const Character &input);
    json toJson(const Ship &input);
    json toJson(const Planet &input);
    json toJson(const PlacedItem &input);
    json toJson(const WalkableArea &input);

    json toJson(const SpaceTransform &input);
}