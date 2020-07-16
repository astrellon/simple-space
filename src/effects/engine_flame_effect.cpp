#include "engine_flame_effect.hpp"

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../utils.hpp"
#include "../game/ship.hpp"

namespace space
{
    EngineFlameEffect::EngineFlameEffect(Ship &parentShip, const sf::Texture &texture, sf::Vector2f position) : _parentShip(parentShip), _glow(texture), _position(position)
    {
        auto size = texture.getSize();
        _glow.setOrigin(size.x / 2, size.y / 2);
    }

    void EngineFlameEffect::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto worldSpace = Utils::transformPoint(_position, parentTransform);
        _glow.setPosition(worldSpace);

        _glow.setRotation(session.engine().camera().view().getRotation());

        auto alpha = _parentShip.moveInput != sf::Vector2f() ? 255 : 0;
        _glow.setColor(sf::Color(255, 255, 255, alpha));
    }
    void EngineFlameEffect::draw(sf::RenderTarget &target)
    {
        target.draw(_glow);
    }
} // namespace space
