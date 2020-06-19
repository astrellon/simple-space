#include "game_session.hpp"

#include "engine.hpp"
#include "game/ship.hpp"
#include "game/planet.hpp"
#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"

namespace space
{
    GameSession::GameSession(Engine &engine) : _engine(engine)
    {

    }
    GameSession::~GameSession()
    {

    }

    Ship *GameSession::createShip(const ObjectId &id, const ShipDefinition &definition)
    {
        auto ship = std::make_unique<Ship>(id, definition);
        auto result = ship.get();
        _spaceObjects.emplace_back(std::move(ship));

        return result;
    }

    Planet *GameSession::createPlanet(const ObjectId &id, const PlanetDefinition &definition)
    {
        auto planet = std::make_unique<Planet>(id, definition);
        auto result = planet.get();
        _spaceObjects.emplace_back(std::move(planet));

        return result;
    }

    bool GameSession::tryGetSpaceObject(const ObjectId &id, SpaceObject **result)
    {
        for (auto &obj : _spaceObjects)
        {
            if (obj->id == id)
            {
                *result = obj.get();
                return true;
            }
        }

        return false;
    }

    void GameSession::update(sf::Time dt)
    {
        for (auto &spaceObject : _spaceObjects)
        {
            spaceObject->update(_engine, dt);
        }
    }

    void GameSession::draw(sf::RenderTarget &target)
    {
        for (auto &spaceObject : _spaceObjects)
        {
            spaceObject->draw(_engine, target, sf::Transform::Identity);
        }
    }
} // namespace town
