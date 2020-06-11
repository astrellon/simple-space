#include "game_session.hpp"

#include "engine.hpp"
#include "game/ship.hpp"

namespace space
{
    GameSession::GameSession(Engine *engine) : _engine(engine)
    {

    }
    GameSession::~GameSession()
    {

    }

    Ship *GameSession::createShip(const sf::Texture *texture, float maxRotation, float maxSpeed)
    {
        auto &ship = _ships.emplace_back(std::make_unique<Ship>(texture, maxRotation, maxSpeed));
        return ship.get();
    }

    void GameSession::onResize(sf::Vector2f area)
    {
        _camera.setSize(area);
    }

    void GameSession::update(sf::Time dt)
    {
        for (auto &ship : _ships)
        {
            ship->update(dt);
        }
    }

    void GameSession::draw(sf::RenderTarget &target)
    {
        for (auto &ship : _ships)
        {
            ship->draw(target);
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
