#pragma once

#include <SFML/Graphics.hpp>
#include "json.hpp"

using nlohmann::json;

namespace space
{
    class GameSession;
    class Engine;
    class SpaceObject;
    class Character;
    class Ship;

    json toJson(const GameSession &input);
    std::unique_ptr<GameSession> fromGameSession(Engine &engine, const json &j);
}