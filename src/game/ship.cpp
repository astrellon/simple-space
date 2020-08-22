#include "ship.hpp"

#include "../utils.hpp"
#include "../game_session.hpp"
#include "walkable_area.hpp"

#include "items/placed_item.hpp"

#include "../physics/polygon_collider.hpp"

#include "../debug/draw_debug.hpp"

namespace space
{
    Ship::Ship(const ObjectId &id, const ShipDefinition &definition) : Ship(id, definition, std::make_unique<WalkableArea>())
    {

    }

    Ship::Ship(const ObjectId &id, const ShipDefinition &definition, std::unique_ptr<WalkableArea> walkableArea):
        SpaceObject(id), definition(definition), _sprite(*definition.texture), _interiorSprite(*definition.interiorTexture), _rotationSpeed(0), rotateInput(0), _walkableArea(std::move(walkableArea))
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
        auto maxSize = std::max(size.x, size.y) * 0.252f;
        _collider->setMainPolygon({{maxSize, -maxSize}, {maxSize, maxSize}, {-maxSize, maxSize}, {-maxSize, -maxSize}});

        // Following polylines define holes.
        if (definition.interiorPolygon.size() <= 2)
        {
            _collider->setHole({{-4, -12}, {4, -12}, {10, 3}, {3, 12}, {-3, 12}, {-10, 3}}, 0);
        }
        else
        {
            _collider->setHole(definition.interiorPolygon, 0);
        }

        _walkableArea->partOfShip(this);
        _walkableArea->addStaticCollider(*_collider);

        if (definition.engineGlowTexture != nullptr)
        {
            auto &texture = *definition.engineGlowTexture;
            for (auto &placement : definition.enginePlacements)
            {
                _engineEffects.emplace_back(std::make_unique<EngineFlameEffect>(*this, texture, placement));
            }
        }
    }

    void Ship::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();
        _rotationSpeed += rotateInput * seconds * definition.turnRate;
        if (_rotationSpeed > definition.maxRotation)
        {
            _rotationSpeed = definition.maxRotation;
        }
        if (_rotationSpeed < -definition.maxRotation)
        {
            _rotationSpeed = -definition.maxRotation;
        }

        _transform.rotation += _rotationSpeed * seconds;

        _speed += Utils::transformDirection(moveInput, _transform.getTransform()) * seconds * definition.acceleration;
        auto length = _speed.x * _speed.x + _speed.y * _speed.y;
        if (length > (definition.maxSpeed * definition.maxSpeed))
        {
            length = std::sqrt(length);
            _speed /= length;
            _speed *= definition.maxSpeed;
        }
        _transform.position += _speed * seconds;

        if (moveInput == sf::Vector2f())
        {
            _speed *= 0.98f;
        }
        if (rotateInput == 0.0f)
        {
            _rotationSpeed *= 0.95f;
        }

        updateWorldTransform(parentTransform);

        _walkableArea->update(session, dt, _worldTransform);

        for (auto &engineEffect : _engineEffects)
        {
            engineEffect->update(session, dt, _worldTransform);
        }
    }

    void Ship::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;

        if (session.isControllingCharacter())
        {
            auto drawPreTeleport = session.drawingPreTeleport();

            if ((session.getShipPlayerIsInsideOf() == this && !drawPreTeleport) ||
                (session.getShipPlayerCloneIsInsideOf() == this && drawPreTeleport))
            {
                target.draw(_interiorSprite, _worldTransform);
                DrawDebug::glDraw++;
                _walkableArea->draw(session, target);
            }
        }

        for (auto &engineEffect : _engineEffects)
        {
            engineEffect->draw(target);
        }

        if (DrawDebug::showPolygons)
        {
            _collider->debugDraw(target, _worldTransform);
        }
    }
    void Ship::onPostLoad(GameSession &session)
    {
        SpaceObject::onPostLoad(session);
        _walkableArea->onPostLoad(session);
    }
}