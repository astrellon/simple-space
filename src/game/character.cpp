#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"
#include "../game_session.hpp"
#include "../space_transform.hpp"
#include "walkable_area.hpp"
#include "../debug/draw_debug.hpp"

namespace space
{
    Character::Character(const ObjectId &id, const CharacterDefinition &definition) :
        SpaceObject(id), definition(definition), rotateInput(0), _physicsBody(nullptr), _insideArea(nullptr), _sprite(*definition.texture), _flipSprite(false)
    {
        _interactable.name(definition.name);
        _sprite.sequence("idle", true);

        _spriteBounds = _sprite.getGlobalBounds();
        _spriteBounds.width *= Utils::getInsideScale();
        _spriteBounds.height *= Utils::getInsideScale();
        _spriteBounds.top *= Utils::getInsideScale();
        _spriteBounds.left *= Utils::getInsideScale();
    }

    void Character::prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();

        if (_insideArea && _insideArea->partOfPlanetSurface())
        {
            auto movement = moveInput * definition.speed;
            b2Vec2 b2movement(movement.x, movement.y);

            _physicsBody->SetLinearVelocity(b2movement);
        }
        else
        {
            auto movement = moveInput * definition.speed * 20.0f;
            b2Vec2 b2movement(movement.x, movement.y);

            _physicsBody->ApplyForceToCenter(b2movement, true);
            _physicsBody->ApplyAngularImpulse(rotateInput, true);
        }
    }

    void Character::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto pos = _physicsBody->GetPosition();
        auto rotation = _physicsBody->GetAngle();

        _transform.position = sf::Vector2f(pos.x, pos.y);
        _transform.rotation = Utils::radiansToDegrees(rotation);

        updateWorldTransform(parentTransform);

        if (_flipSprite)
        {
            _worldTransform *= sf::Transform(-1, 0, 0,  0, 1, 0,  0, 0, 1);
        }

        _sprite.update(dt);
    }

    void Character::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;

        if (DrawDebug::showPolygons)
        {
            sf::RectangleShape shape;
            shape.setFillColor(sf::Color(100, 120, 255, 120));
            shape.setSize(sf::Vector2f(20, 20));
            shape.setPosition(sf::Vector2f(-10, -10));
            target.draw(shape, _worldTransform);
        }
    }
    bool Character::doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const
    {
        auto worldPos = Utils::getPosition(_worldTransform);
        auto local = mousePosition - worldPos;
        return _spriteBounds.contains(local);
    }

    void Character::addToPhysicsWorld(b2World *world)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.angularDamping = 0.5f;
        bodyDef.linearDamping = 0.5f;

        b2PolygonShape shape;
        shape.SetAsBox(1, 1);

        b2FixtureDef fixtureDef;

        fixtureDef.restitution = 0;
        fixtureDef.friction = 0.9f;
        fixtureDef.density = 5.0f;
        fixtureDef.shape = &shape;

        if (!isInSpace())
        {
            bodyDef.fixedRotation = true;
            _transform.rotation = 0;
        }

        _physicsBody = world->CreateBody(&bodyDef);
        _physicsBody->CreateFixture(&fixtureDef);
        _physicsBody->SetTransform(b2Vec2(_transform.position.x, _transform.position.y), Utils::degreesToRadians(_transform.rotation));

        _physicsBody->SetUserData(this);
    }
    void Character::removeFromPhysicsWorld(b2World *world)
    {
        world->DestroyBody(_physicsBody);
        _physicsBody = nullptr;
    }

    bool Character::isInSpace() const
    {
        return _insideArea && _insideArea->partOfShip();
    }
} // namespace space
