#include "ship.hpp"

#include "../utils.hpp"
#include "../game_session.hpp"
#include "walkable_area.hpp"

#include "items/placed_item.hpp"

#include "../physics/polygon_collider.hpp"

namespace space
{
    Ship::Ship(const ObjectId &id, const ShipDefinition &definition):
        SpaceObject(id), definition(definition), _sprite(*definition.texture), _interiorSprite(*definition.interiorTexture), _rotationSpeed(0), rotateInput(0)
    {

        auto size = definition.texture->getSize();
        _sprite.setOrigin(size.x / 2, size.y / 2);

        size = definition.interiorTexture->getSize();
        _interiorSprite.setOrigin(size.x / 2, size.y / 2);
        _interiorSprite.setScale(Utils::getInsideScale(), Utils::getInsideScale());
        _interiorSprite.move(sf::Vector2f(definition.interiorTextureOffset) * 0.25f);

        _collider = std::make_unique<PolygonCollider>(b2_staticBody);

        // Fill polygon structure with actual data. Any winding order works.
        // The first polyline defines the main polygon.
        _collider->setMainPolygon({{50, -50}, {50, 50}, {-50, 50}, {-50, -50}});
        // Following polylines define holes.
        _collider->setHole({{-4, -12}, {4, -12}, {10, 3}, {3, 12}, {-3, 12}, {-10, 3}}, 0);

        _walkableArea.partOfShip(this);
        _walkableArea.addStaticCollider(*_collider);
    }

    void Ship::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();
        _speed += Utils::transformDirection(moveInput, _transform.getTransform()) * seconds * definition.acceleration;
        _rotationSpeed += rotateInput * seconds * definition.turnRate;

        _transform.position += _speed * seconds;
        _transform.rotation += _rotationSpeed * seconds;

        if (moveInput == sf::Vector2f())
        {
            _speed *= 0.98f;
        }
        if (rotateInput == 0.0f)
        {
            _rotationSpeed *= 0.98f;
        }

        updateWorldTransform(parentTransform);

        _walkableArea.update(session, dt, _worldTransform);
    }

    void Ship::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);

        if (session.getShipPlayerIsInsideOf() == this && session.isControllingCharacter())
        {
            target.draw(_interiorSprite, _worldTransform);
            _walkableArea.draw(session, target);
        }

        //_collider->debugDraw(target, _worldTransform);
    }
}