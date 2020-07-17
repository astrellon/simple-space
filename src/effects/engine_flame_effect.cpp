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

        auto engineOn = _parentShip.moveInput != sf::Vector2f();
        auto seconds = dt.asSeconds();

        _engineBrightness += engineOn ? seconds * 10.0f : seconds * -4.0f;
        _engineBrightness = Utils::clamp01(_engineBrightness);

        if (_engineBrightness >= 0.0f)
        {
            auto time = session.engine().timeSinceStart().asSeconds();
            auto flux = Utils::clamp01(_engineBrightness - Utils::perlin(time * 7.0f) * 0.3f);

            _glow.setColor(sf::Color(255, 255, 255, flux * 255));
        }
    }
    void EngineFlameEffect::draw(sf::RenderTarget &target)
    {
        if (_engineBrightness > 0.0f)
        {
            target.draw(_glow);
        }
    }
} // namespace space
