#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"
#include "../game_session.hpp"
#include "../space_transform.hpp"
#include "../debug/draw_debug.hpp"
#include "../render_camera.hpp"
#include "ship.hpp"
#include "area.hpp"

namespace space
{
    const SpaceObjectType2 Character::TypeValue = SpaceObjectType2::Character;

    Character::Character(const ObjectId &id, const CharacterDefinition &definition, SpaceObjectType2 typeValue) :
        SpaceObject(id, typeValue), definition(definition), rotateInput(0), _physicsBody(nullptr), _sprite(*definition.texture)
    {
        createInteractable();
        _interactable->name(definition.name);
        _sprite.sequence("idle", true);

        _spriteBounds = _sprite.getGlobalBounds();
        _spriteBounds.width *= Utils::InsideScale;
        _spriteBounds.height *= Utils::InsideScale;
        _spriteBounds.top *= Utils::InsideScale;
        _spriteBounds.left *= Utils::InsideScale;
    }

    Character *Character::cloneCharacter(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<Character>(newId, definition);
        result->flipSprite(_flipSprite);
        populateCloneFromThis(result, context);

        result->sprite().sequence(_sprite.currentAnimation(), true);
        result->sprite().animationTime = _sprite.animationTime;

        return result;
    }

    void Character::insideArea(Area *area)
    {
        if (_insideArea != nullptr)
        {
            auto physicsWorld = _insideArea->physicsWorld();
            if (physicsWorld)
            {
                removeFromPhysicsWorld(physicsWorld);
            }
        }

        SpaceObject::insideArea(area);

        if (area)
        {
            auto physicsWorld = area->physicsWorld();
            if (physicsWorld)
            {
                addToPhysicsWorld(physicsWorld);
            }
        }
    }

    void Character::prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        if (_partOfLivePhoto)
        {
            return;
        }

        if (_insideArea && _insideArea->type() == AreaType::PlanetSurface)
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
        if (!_partOfLivePhoto)
        {
            applyPhysicsToTransform();
        }

        updateWorldTransform(parentTransform);

        if (_flipSprite)
        {
            _worldTransform *= sf::Transform(-1, 0, 0,  0, 1, 0,  0, 0, 1);
        }

        _sprite.update(dt);
    }

    void Character::draw(GameSession &session, RenderCamera &target)
    {
        target.texture().draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;

        if (!_partOfLivePhoto && DrawDebug::showPolygons)
        {
            sf::RectangleShape shape;
            shape.setFillColor(sf::Color(100, 120, 255, 120));
            shape.setSize(sf::Vector2f(20, 20));
            shape.setPosition(sf::Vector2f(-10, -10));
            target.texture().draw(shape, _worldTransform);
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
        if (_partOfLivePhoto)
        {
            return;
        }

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.angularDamping = 0.5f;
        bodyDef.linearDamping = 0.5f;

        b2PolygonShape shape;
        shape.SetAsBox(2, 2);

        b2FixtureDef fixtureDef;

        fixtureDef.restitution = 0;
        fixtureDef.friction = 0.9f;
        fixtureDef.density = 0.5f;
        fixtureDef.shape = &shape;

        if (!isInSpace())
        {
            bodyDef.fixedRotation = true;
            _transform.rotation = 0;
        }

        _physicsBody = world->CreateBody(&bodyDef);
        _physicsBody->CreateFixture(&fixtureDef);
        _physicsBody->SetTransform(b2Vec2(_transform.position.x, _transform.position.y), Utils::degreesToRadians(_transform.rotation));

        _physicsBody->GetUserData().pointer = (uintptr_t)this;
    }
    void Character::removeFromPhysicsWorld(b2World *world)
    {
        if (_partOfLivePhoto)
        {
            return;
        }

        world->DestroyBody(_physicsBody);
        _physicsBody = nullptr;
    }

    bool Character::isInSpace() const
    {
        return _insideArea && _insideArea->type() == AreaType::Ship;
    }

    void Character::applyPhysicsToTransform()
    {
        auto pos = _physicsBody->GetPosition();
        auto rotation = _physicsBody->GetAngle();

        _transform.position = sf::Vector2f(pos.x, pos.y);
        _transform.rotation = Utils::radiansToDegrees(rotation);
    }
} // namespace space
