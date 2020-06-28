#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"
#include "../engine.hpp"
#include "../space_transform.hpp"

namespace space
{
    Character::Character(const ObjectId &id, const CharacterDefinition &definition) :
        SpaceObject(id), definition(definition), _rotationSpeed(0), rotateInput(0), _physicsBody(nullptr), _physicsBodyDef(), _physicsFixureDef()
    {
        _physicsBodyDef.type = b2_dynamicBody;
        _physicsBodyDef.angularDamping = 0.5f;
        _physicsBodyDef.linearDamping = 0.5f;

        auto shape = new b2PolygonShape();
        shape->SetAsBox(1.0f, 1.0f);
        // auto shape = new b2CircleShape();
        // shape->m_radius = 1.5f;
        _physicsFixureDef.shape = shape;
        _physicsFixureDef.restitution = 0;
        _physicsFixureDef.friction = 0.9f;
        _physicsFixureDef.density = 5.0f;
    }

    void Character::prePhysics(Engine &engine, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();

        auto movement = moveInput / definition.speed * 10000.0f;
        _physicsBody->ApplyForceToCenter(b2Vec2(movement.x, movement.y), true);
        //_physicsBody->SetLinearVelocity(b2Vec2(movement.x, movement.y));
        _physicsBody->ApplyAngularImpulse(rotateInput * 30.0f, true);
        // _rotationSpeed += rotateInput * seconds * 30.0f;

        // _transform.position += movement;
        // _transform.rotation += _rotationSpeed * seconds;

        //updateWorldTransform(parentTransform);
    }
    void Character::update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto pos = _physicsBody->GetPosition();
        auto rotation = _physicsBody->GetAngle();

        _transform.position = sf::Vector2f(pos.x, pos.y);
        _transform.rotation = rotation;

        updateWorldTransform(parentTransform);
    }

    void Character::draw(Engine &engine, sf::RenderTarget &target)
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
        _physicsBody->CreateFixture(&_physicsFixureDef);

        std::cout << "Mass: " << _physicsBody->GetMass() << std::endl;
    }
    void Character::removeFromPhysicsWorld(b2World *world)
    {
        world->DestroyBody(_physicsBody);
        _physicsBody = nullptr;
    }
} // namespace space
