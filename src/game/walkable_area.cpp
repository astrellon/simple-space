#include "walkable_area.hpp"

#include "engine.hpp"
#include "game/items/placeable_item.hpp"
#include "game/character.hpp"
#include "physics/polygon_collider.hpp"
#include "utils.hpp"
#include "game_session.hpp"

namespace space
{
    WalkableArea::WalkableArea() : _physicsWorld(b2Vec2(0, 0)), _partOfShip(nullptr)
    {
        _physicsWorld.SetContactListener(this);
    }
    WalkableArea::~WalkableArea()
    {
        for (auto &character : _characters)
        {
            character->removeFromPhysicsWorld(&_physicsWorld);
        }
    }

    void WalkableArea::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        _worldTransform = parentTransform;
        _session = &session;

        for (auto &placedItem : _placedItems)
        {
            placedItem->update(session, dt, _worldTransform);
        }

        for (auto &character : _characters)
        {
            character->prePhysics(session, dt, parentTransform);
        }

        _physicsWorld.Step(1.0f / 60.0f, 4, 2);

        for (auto &character : _characters)
        {
            character->update(session, dt, parentTransform);
        }
    }

    void WalkableArea::draw(GameSession &session, sf::RenderTarget &target)
    {
        for (auto &placedItem : _placedItems)
        {
            placedItem->draw(session, target);
        }

        for (auto &character : _characters)
        {
            character->draw(session, target);
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

    void WalkableArea::addPlaceable(PlaceableItem *item, sf::Vector2f position)
    {
        _placedItems.emplace_back(std::make_unique<PlacedItem>(item, position, *this));
        auto &placedItem = _placedItems.back();
        placedItem->addPhysics(_physicsWorld);
    }
    void WalkableArea::removePlaceable(ItemId id)
    {
        for (auto iter = _placedItems.begin(); iter != _placedItems.end(); ++iter)
        {
            if (iter->get()->item->id == id)
            {
                _placedItems.erase(iter);
                iter->get()->removePhysics(_physicsWorld);
                return;
            }
        }

        std::cout << "Unable to find placeable item to remove it from walkable area: " << id << std::endl;
    }

    void WalkableArea::BeginContact(b2Contact *contact)
    {
        std::cout << "Contact started" << std::endl;

        auto userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        auto userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        if (userDataA == nullptr || userDataB == nullptr)
        {
            return;
        }

        auto spaceObjectA = static_cast<SpaceObject *>(userDataA);
        auto spaceObjectB = static_cast<SpaceObject *>(userDataB);

        auto item = dynamic_cast<PlacedItem *>(spaceObjectB);
        auto character = dynamic_cast<Character *>(spaceObjectA);

        if (character == nullptr)
        {
            character = dynamic_cast<Character *>(spaceObjectB);
            item = dynamic_cast<PlacedItem *>(spaceObjectA);
        }

        if (character == nullptr)
        {
            std::cout << "No character in contact, ignoring" << std::endl;
            return;
        }

        _session->playerController().addCanInteractWith(item);

        std::cout << "Character can interact with " << item->item->definition.name << std::endl;
    }
    void WalkableArea::EndContact(b2Contact *contact)
    {
        auto userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        auto userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        if (userDataA == nullptr || userDataB == nullptr)
        {
            return;
        }

        auto spaceObjectA = static_cast<SpaceObject *>(userDataA);
        auto spaceObjectB = static_cast<SpaceObject *>(userDataB);

        auto item = dynamic_cast<PlacedItem *>(spaceObjectB);
        auto character = dynamic_cast<Character *>(spaceObjectA);

        if (character == nullptr)
        {
            character = dynamic_cast<Character *>(spaceObjectB);
            item = dynamic_cast<PlacedItem *>(spaceObjectA);
        }

        if (character == nullptr)
        {
            std::cout << "No character in contact, ignoring" << std::endl;
            return;
        }

        _session->playerController().removeCanInteractWith(item);

        std::cout << "Character outside range of interacting with " << item->item->definition.name << std::endl;
    }
} // namespace space
