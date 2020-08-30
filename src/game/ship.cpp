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

        createMainCollider();
        for (auto &points : definition.extraInteriorPolygons)
        {
            createExtraCollider(points);
        }

        _walkableArea->partOfShip(this);

        for (auto &collider : _colliders)
        {
            _walkableArea->addStaticCollider(*collider);
        }

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
        _speed = Utils::clampLength(_speed, 0, definition.maxSpeed);
        _transform.position += _speed * seconds;

        if (moveInput == sf::Vector2f() && _speed != sf::Vector2f())
        {
            auto length = Utils::length(_speed);
            auto prevSpeed = _speed;
            auto prevSpeedDir = _speed / length;

            _speed -= prevSpeedDir * definition.acceleration * seconds;

            auto newSpeedDir = Utils::normalised(_speed);
            auto dot = Utils::dot(prevSpeedDir, newSpeedDir);
            if (std::abs(dot - 1) > 0.01)
            {
                _speed = sf::Vector2f();
            }
        }

        if (rotateInput == 0.0f && _rotationSpeed != 0.0f)
        {
            auto prevSpeed = _rotationSpeed;
            auto prevSign = _rotationSpeed > 0 ? 1 : -1;
            _rotationSpeed -= prevSign * definition.turnRate * seconds;
            auto newSign = _rotationSpeed > 0 ? 1 : -1;

            if (prevSign != newSign)
            {
                _rotationSpeed = 0.0f;
            }
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
            for (auto &collider : _colliders)
            {
                collider->debugDraw(target, _worldTransform);
            }
        }
    }
    void Ship::onPostLoad(GameSession &session)
    {
        SpaceObject::onPostLoad(session);
        _walkableArea->onPostLoad(session);
    }

    void Ship::createMainCollider()
    {
        auto size = definition.interiorTexture->getSize();
        auto &collider = _colliders.emplace_back(std::make_unique<PolygonCollider>(b2_staticBody));

        // Fill polygon structure with actual data. Any winding order works.
        // The first polyline defines the main polygon.
        auto maxSize = std::max(size.x, size.y) * Utils::getInsideScale();
        collider->setMainPolygon({{maxSize, -maxSize}, {maxSize, maxSize}, {-maxSize, maxSize}, {-maxSize, -maxSize}});

        // Following polylines define holes.
        if (definition.interiorPolygon.size() <= 2)
        {
            collider->setHole({{-4, -12}, {4, -12}, {10, 3}, {3, 12}, {-3, 12}, {-10, 3}}, 0);
        }
        else
        {
            collider->setHole(definition.interiorPolygon, 0);
        }
    }

    void Ship::createExtraCollider(const Points &points)
    {
        auto &collider = _colliders.emplace_back(std::make_unique<PolygonCollider>(b2_staticBody));

        collider->setMainPolygon(points);
    }
}