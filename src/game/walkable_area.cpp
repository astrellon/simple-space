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

    }
    WalkableArea::~WalkableArea()
    {
        for (auto &character : _characters)
        {
            character->removeFromPhysicsWorld(&_physicsWorld);
        }

        for (auto &placedItem : _placedItems)
        {
            placedItem->removePhysics(_physicsWorld);
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

        auto controllingCharacter = session.playerController().controllingCharacter();
        if (controllingCharacter != nullptr && controllingCharacter->insideArea() == this)
        {
            checkForInteractables(session);
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
                if (_session != nullptr)
                {
                    _session->playerController().removeCanInteractWith(iter->get());
                }
                iter->get()->removePhysics(_physicsWorld);
                _placedItems.erase(iter);
                return;
            }
        }

        std::cout << "Unable to find placeable item to remove it from walkable area: " << id << std::endl;
    }

    void WalkableArea::checkForInteractables(GameSession &session)
    {
        auto &player = session.playerController();

        // Check existing items
        auto playerPos = player.controllingCharacter()->transform().position;
        for (auto item : player.canInteractWith())
        {
            auto dpos = item->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - item->interactRadiusSquared() > 0.0f)
            {
                player.removeCanInteractWith(item);
            }
        }

        for (auto &iter : _placedItems)
        {
            if (player.canInteractWith(iter.get()))
            {
                continue;
            }

            auto dpos = iter->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - iter->interactRadiusSquared() < 0.0f)
            {
                player.addCanInteractWith(iter.get());
            }
        }
    }
} // namespace space
