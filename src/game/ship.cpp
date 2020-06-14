#include "ship.hpp"

#include "../utils.hpp"

namespace space
{
    Ship::Ship(const ObjectId &id, const ShipDefinition &definition):
        SpaceObject(id), definition(definition), _sprite(*definition.texture), _rotationSpeed(0), rotateInput(0)
    {
        auto size = definition.texture->getSize();
        _sprite.setOrigin(size.x / 2, size.y / 2);
    }

    void Ship::update(sf::Time dt)
    {
        auto seconds = dt.asSeconds();
        _speed += Utils::transformDirection(moveInput, _transform.getTransform());
        _rotationSpeed += rotateInput;

        _transform.position += _speed * seconds;
        _transform.rotation += _rotationSpeed * seconds;
    }

    void Ship::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        const auto combinedTransform = parentTransform * _transform.getTransform();
        target.draw(_sprite, combinedTransform);
    }
}