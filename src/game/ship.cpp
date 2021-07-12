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
    const SpaceObjectType Ship::TypeValue = SpaceObjectType::Ship;

    Ship::Ship(const ObjectId &id, const ShipDefinition &definition):
        SpaceObject(id),
        definition(definition),
        rotateInput(0),
        disableRender(false),
        _area(AreaType::Ship, this),
        _sprite(*definition.texture),
        _interiorSprite(*definition.interiorTexture),
        _rotationSpeed(0)
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
        result->rotateInput = rotateInput;
        result->moveInput = moveInput;

        if (Utils::contains(context.showingAreas, &_area))
        {
            _area.cloneInto(newIdPrefix, result->area(), context);
        }

        return result;
    }

    Ship *Ship::cloneShip(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<Ship>(newId, definition);
        populateCloneFromThis(result, context);
        return result;
    }

    void Ship::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        if (!_partOfLivePhoto)
        {
            processInputs(dt);
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

    bool Ship::doesMouseHover(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition) const
    {
        if (inRelationTo && inRelationTo->partOfShip() == this)
        {
            return _area.checkForMouse(inRelationTo, session, mousePosition);
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

    bool Ship::loopOver(LoopObjectCallback callback)
    {
        if (!SpaceObject::loopOver(callback))
        {
            return false;
        }
        if (!_area.loopOver(callback))
        {
            return false;
        }
        return true;
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

    void Ship::processInputs(sf::Time dt)
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
            auto prevSign = _rotationSpeed > 0 ? 1 : -1;
            _rotationSpeed -= prevSign * definition.turnRate * seconds;
            auto newSign = _rotationSpeed > 0 ? 1 : -1;

            if (prevSign != newSign)
            {
                _rotationSpeed = 0.0f;
            }
        }
    }
}