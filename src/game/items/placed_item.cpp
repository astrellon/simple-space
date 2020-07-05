#include "placed_item.hpp"

#include "../../engine.hpp"
#include "placeable_item.hpp"
#include "../character.hpp"
#include "../../physics/polygon_collider.hpp"
#include "../../utils.hpp"

namespace space
{
    PlacedItem::PlacedItem(PlaceableItem const *item, const sf::Vector2f &position) : SpaceObject(Utils::makeObjectId(item->id)), item(item), _sprite(*item->definition.texture), _triggerSensor(nullptr)
    {
        _transform.position = position;
        _sprite.setPosition(position);

        _sprite.setOrigin(sf::Vector2f(item->definition.texture->getSize()) * 0.5f);
        _sprite.setScale(Utils::getInsideScale(), Utils::getInsideScale());
    }
    PlacedItem::~PlacedItem()
    {
        assert(_triggerSensor != nullptr);
    }

    void PlacedItem::addPhysics(b2World &world)
    {
        b2CircleShape shape;
        shape.m_radius = 5.0f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.isSensor = true;

        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = b2Vec2(_transform.position.x, _transform.position.y);

        _triggerSensor = world.CreateBody(&bodyDef);
        _triggerSensor->CreateFixture(&fixtureDef);
        _triggerSensor->SetUserData(this);
    }
    void PlacedItem::removePhysics(b2World &world)
    {
        world.DestroyBody(_triggerSensor);
        _triggerSensor = nullptr;
    }

    void PlacedItem::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }

    void PlacedItem::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
    }
}