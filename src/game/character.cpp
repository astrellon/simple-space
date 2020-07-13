#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"
#include "../game_session.hpp"
#include "../space_transform.hpp"

namespace space
{
    Character::Character(const ObjectId &id, const CharacterDefinition &definition) :
        SpaceObject(id), definition(definition), _rotationSpeed(0), rotateInput(0), _physicsBody(nullptr), _insideArea(nullptr)
    {
        _physicsBodyDef.type = b2_dynamicBody;
        _physicsBodyDef.angularDamping = 0.5f;
        _physicsBodyDef.linearDamping = 0.5f;

        auto shape = new b2PolygonShape();
        shape->SetAsBox(1.0f, 1.0f);
        _physicsFixtureDef.restitution = 0;
        _physicsFixtureDef.friction = 0.9f;
        _physicsFixtureDef.density = 5.0f;
        _physicsFixtureDef.shape = shape;
    }

    void Character::prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();

        auto movement = moveInput / definition.speed * 50000.0f;

        _physicsBody->ApplyForceToCenter(b2Vec2(movement.x, movement.y), true);
        _physicsBody->ApplyAngularImpulse(rotateInput, true);
    }
    void Character::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto pos = _physicsBody->GetPosition();
        auto rotation = _physicsBody->GetAngle();

        _transform.position = sf::Vector2f(pos.x, pos.y);
        _transform.rotation = Utils::radiansToDegrees(rotation);

        updateWorldTransform(parentTransform);
    }

    void Character::draw(GameSession &session, sf::RenderTarget &target)
    {
        auto elapsedTime = _timeSinceStart.getElapsedTime().asSeconds() * 8;
        auto mod = std::fmod(elapsedTime, static_cast<double>(definition.tiles.length()));
        auto index = static_cast<uint>(mod);

        auto sprite = definition.tiles.sprite(index);
        target.draw(*sprite, _worldTransform);
    }

    void Character::addToPhysicsWorld(b2World *world)
    {
        _physicsBody = world->CreateBody(&_physicsBodyDef);
        _physicsBody->CreateFixture(&_physicsFixtureDef);
        _physicsBody->SetTransform(b2Vec2(_transform.position.x, _transform.position.y), Utils::degreesToRadians(_transform.rotation));

        _physicsBody->SetUserData(this);
    }
    void Character::removeFromPhysicsWorld(b2World *world)
    {
        world->DestroyBody(_physicsBody);
        _physicsBody = nullptr;
    }
} // namespace space
