#include "game_session.hpp"

#include "engine.hpp"
#include "game/ship.hpp"
#include "definitions/ship_definition.hpp"

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
            spaceObject->update(dt);
        }
    }

    void GameSession::draw(sf::RenderTarget &target)
    {
        for (auto &spaceObject : _spaceObjects)
        {
            spaceObject->draw(target);
        }
        // if (_currentMap != nullptr)
        // {
        //     auto scale = _engine->spriteScale() * _engine->spriteSize();
        //     auto head_position = _player.headPosition();
        //     sf::Vector2f cam_position = sf::Vector2f(_currentMap->width() * scale * 0.5f, _currentMap->height() * scale * 0.5f);

        //     _camera.setCenter(cam_position);
        //     target.setView(_camera);

        //     _currentMap->draw(_engine, target);
        //     _player.draw(_engine, target);
        // }
    }
} // namespace town
