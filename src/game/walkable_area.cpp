#include "walkable_area.hpp"

#include "engine.hpp"
#include "game/items/placeable_item.hpp"
#include "game/character.hpp"
#include "physics/polygon_collider.hpp"
#include "utils.hpp"

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



    WalkableArea::WalkableArea() : _physicsWorld(b2Vec2(0, 0)), _partOfShip(nullptr)
    {
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

    void WalkableArea::addStaticCollider(PolygonCollider &collider)
    {
        collider.addToWorld(&_physicsWorld);
    }
    void WalkableArea::removeStaticCollider(PolygonCollider &collider)
    {
        collider.removeFromWorld(&_physicsWorld);
    }

    void WalkableArea::addCharacter(Character *character)
    {
        if (character->insideArea() != nullptr)
        {
            character->insideArea()->removeCharacter(character);
        }

        character->transform().scale = Utils::getInsideScale();
        _characters.push_back(character);

        character->addToPhysicsWorld(&_physicsWorld);
        character->insideArea(this);
    }
    void WalkableArea::removeCharacter(Character *character)
    {
        auto find = std::find(_characters.begin(), _characters.end(), character);
        if (find != _characters.end())
        {
            _characters.erase(find);
        }

        character->removeFromPhysicsWorld(&_physicsWorld);
        character->insideArea(nullptr);
    }
} // namespace space
