#include "ship.hpp"

namespace space
{
    Ship::Ship(const ObjectId &id, const ShipDefinition &definition):
        SpaceObject(id), definition(definition), _sprite(*definition.texture)
    {
        auto size = definition.texture->getSize();
        _sprite.setOrigin(size.x / 2, size.y / 2);
    }

    void Ship::update(sf::Time dt)
    {
        _transform.rotate(dt.asSeconds() * 15);
    }

    void Ship::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        const auto combinedTransform = parentTransform * _transform;
        target.draw(_sprite, combinedTransform);
    }
}