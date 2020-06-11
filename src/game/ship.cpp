#include "ship.hpp"

namespace space
{
    void Ship::update(sf::Time dt)
    {
        _transform.rotate(dt.asSeconds() * 15);
    }

    void Ship::draw(sf::RenderTarget &target)
    {
        sf::RenderStates state;
        state.transform = _transform;

        sf::Sprite sprite;
        sprite.setTexture(*_texture);
        sprite.setOrigin(32, 32);

        target.draw(sprite, state);
    }
}