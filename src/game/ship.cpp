#include "ship.hpp"

#include "../utils.hpp"
#include "../game_session.hpp"
#include "../physics/polygon_collider.hpp"
#include "../debug/draw_debug.hpp"
#include "../render_camera.hpp"
#include "../controllers/player_controller.hpp"
#include "character.hpp"
#include "items/placed_item.hpp"
#include "area.hpp"

namespace space
{
    Ship::Ship(const ObjectId &id, const ShipDefinition &definition):
        SpaceObject(id), definition(definition), _sprite(*definition.texture), _interiorSprite(*definition.interiorTexture), _rotationSpeed(0), rotateInput(0), _area(AreaType::Ship, this), disableRender(false)
    {
        auto size = definition.texture->getSize();
        _sprite.setOrigin(size.x / 2, size.y / 2);
        _spriteBounds = _sprite.getGlobalBounds();

        size = definition.interiorTexture->getSize();
        _interiorSprite.setOrigin(size.x / 2, size.y / 2);
        _interiorSprite.setScale(Utils::InsideScale, Utils::InsideScale);
        _interiorSprite.move(sf::Vector2f(definition.interiorTextureOffset) * 0.25f);

        createMainCollider();
        for (auto &points : definition.extraInteriorPolygons)
        {
            createExtraCollider(points);
        }

        for (auto &collider : _colliders)
        {
            _area.addStaticCollider(*collider);
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

    SpaceObject *Ship::deepClone(const ObjectId &newIdPrefix, const CloneContext &context)
    {
        auto result = cloneShip(newIdPrefix + id, context);

        _area.cloneInto(newIdPrefix, result->area(), context);

        return result;
    }

    Ship *Ship::cloneShip(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<Ship>(newId, definition);
        result->transform(_transform);
        return result;
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
        _speed = _speed.clampLength(0, definition.maxSpeed);
        _prevPosition = _transform.position;
        _transform.position += _speed * seconds;

        if (moveInput == sf::Vector2f() && _speed != sf::Vector2f())
        {
            auto length = _speed.length();
            auto prevSpeed = _speed;
            auto prevSpeedDir = _speed / length;

            _speed -= prevSpeedDir * definition.acceleration * seconds;

            auto newSpeedDir = _speed.normalised();
            auto dot = prevSpeedDir.dot(newSpeedDir);
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

        _area.update(session, dt, _worldTransform);

        for (auto &engineEffect : _engineEffects)
        {
            engineEffect->update(session, dt, _worldTransform);
        }
    }

    void Ship::draw(GameSession &session, RenderCamera &target)
    {
        if (disableRender)
        {
            return;
        }

        target.texture().draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;

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
    void Ship::onPostLoad(GameSession &session, LoadingContext &context)
    {
        SpaceObject::onPostLoad(session, context);
        _area.onPostLoad(session, context);
    }

    bool Ship::doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const
    {
        auto &controller = session.playerController();
        if (controller.controlling() == ControlCharacter && controller.controllingCharacter()->insideArea()->partOfShip() == this)
        {
            return _area.checkForMouse(session, mousePosition);
        }
        else
        {
            auto worldPos = Utils::getPosition(_worldTransform);
            auto local = mousePosition - worldPos;
            return _spriteBounds.contains(local);
        }
    }

    void Ship::drawInterior(GameSession &session, RenderCamera &target)
    {
        target.texture().draw(_interiorSprite, _worldTransform);
        DrawDebug::glDraw++;
        _area.draw(session, target);
    }

    void Ship::createMainCollider()
    {
        auto size = definition.interiorTexture->getSize();
        auto &collider = _colliders.emplace_back(std::make_unique<PolygonCollider>(b2_staticBody));

        // Fill polygon structure with actual data. Any winding order works.
        // The first polyline defines the main polygon.
        auto maxSize = std::max(size.x, size.y) * Utils::InsideScale;
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

    void Ship::createExtraCollider(const Polygon &polygon)
    {
        auto &collider = _colliders.emplace_back(std::make_unique<PolygonCollider>(b2_staticBody));

        collider->setMainPolygon(polygon);
    }
}