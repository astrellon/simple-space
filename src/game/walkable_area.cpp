#include "walkable_area.hpp"

#include "engine.hpp"
#include "game/placeable_item.hpp"
#include "game/character.hpp"

namespace space
{
    PlacedItem::PlacedItem(PlaceableItem const &item, const sf::Vector2f &position) :
        item(item), position(position), _sprite(*item.definition.texture)
    {
        _sprite.setPosition(position);
    }

    void PlacedItem::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        target.draw(_sprite, parentTransform);
    }



    WalkableArea::WalkableArea() : _physicsWorld(b2Vec2(0, 0))
    {
        auto _physicsFixureDef = new b2FixtureDef();
        auto shape = new b2PolygonShape();
        shape->SetAsBox(32.0f, 1.0f);

        _physicsFixureDef->shape = shape;
        _physicsFixureDef->restitution = 0.0f;
        _physicsFixureDef->friction = 0.9f;

        auto _physicsBodyDef = new b2BodyDef();
        _physicsBodyDef->type = b2_staticBody;
        _physicsBodyDef->position = b2Vec2(0, -2.0f / 100.0f);

        auto body = _physicsWorld.CreateBody(_physicsBodyDef);
        body->CreateFixture(_physicsFixureDef);
    }
    WalkableArea::~WalkableArea()
    {
        for (auto &character : _characters)
        {
            character->removeFromPhysicsWorld(&_physicsWorld);
        }
    }

    void WalkableArea::update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform)
    {
        _worldTransform = parentTransform;

        for (auto &character : _characters)
        {
            character->prePhysics(engine, dt, parentTransform);
        }

        _physicsWorld.Step(1.0f / 60.0f, 4, 2);

        for (auto &character : _characters)
        {
            character->update(engine, dt, parentTransform);
        }
    }

    void WalkableArea::draw(Engine &engine, sf::RenderTarget &target)
    {
        for (auto &placedItem : _placedItems)
        {
            placedItem.draw(target, _worldTransform);
        }

        for (auto &character : _characters)
        {
            character->draw(engine, target);
        }
    }

    void WalkableArea::addCharacter(Character *character)
    {
        character->transform().scale = 0.2f;
        _characters.push_back(character);

        character->addToPhysicsWorld(&_physicsWorld);
    }
    void WalkableArea::removeCharacter(Character *character)
    {
        auto find = std::find(_characters.begin(), _characters.end(), character);
        if (find != _characters.end())
        {
            _characters.erase(find);
        }

        character->removeFromPhysicsWorld(&_physicsWorld);
    }
} // namespace space
