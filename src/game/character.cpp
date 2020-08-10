#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"
#include "../game_session.hpp"
#include "../space_transform.hpp"
#include "walkable_area.hpp"

namespace space
{
    Character::Character(const ObjectId &id, const CharacterDefinition &definition) :
        SpaceObject(id), definition(definition), _rotationSpeed(0), rotateInput(0), _physicsBody(nullptr), _insideArea(nullptr), _tileIndex(0)
    {
        _interactable.name(definition.name);
    }

    void Character::prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();


        if (_insideArea && _insideArea->partOfPlanetSurface())
        {
            auto movement = moveInput / definition.speed * 2500.0f;
            b2Vec2 b2movement(movement.x, movement.y);

            _physicsBody->SetLinearVelocity(b2movement);
        }
        else
        {
            auto movement = moveInput / definition.speed * 50000.0f;
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

        auto elapsedTime = _timeSinceStart.getElapsedTime().asSeconds() * 8;
        auto mod = std::fmod(elapsedTime, static_cast<double>(definition.tiles.length()));
        _tileIndex = static_cast<uint>(mod);
    }

    void Character::draw(GameSession &session, sf::RenderTarget &target)
    {
        auto sprite = definition.tiles.sprite(_tileIndex);
        target.draw(*sprite, _worldTransform);
    }

    void Character::addToPhysicsWorld(b2World *world)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.angularDamping = 0.5f;
        bodyDef.linearDamping = 0.5f;

        b2PolygonShape shape;
        shape.SetAsBox(1.0f, 1.0f);

        b2FixtureDef fixtureDef;

        fixtureDef.restitution = 0;
        fixtureDef.friction = 0.9f;
        fixtureDef.density = 5.0f;
        fixtureDef.shape = &shape;

        if (_insideArea && _insideArea->partOfPlanetSurface())
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
} // namespace space
