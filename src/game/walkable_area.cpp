#include "walkable_area.hpp"

#include "../engine.hpp"
#include "items/placeable_item.hpp"
#include "items/teleporter.hpp"
#include "character.hpp"
#include "../physics/polygon_collider.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"

namespace space
{
    WalkableArea::WalkableArea() : _physicsWorld(b2Vec2(0, 0)), _partOfShip(nullptr), _partOfPlanet(nullptr)
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

    std::vector<PlacedItemPair<Teleporter>> WalkableArea::findTeleporters() const
    {
        std::vector<PlacedItemPair<Teleporter>> result;

        for (auto &placedItem : _placedItems)
        {
            auto teleporter = dynamic_cast<Teleporter *>(placedItem.get()->item);
            if (teleporter == nullptr)
            {
                continue;
            }

            result.push_back(PlacedItemPair<Teleporter>(placedItem.get(), teleporter));
        }

        return result;
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
        // Snap position to the inside pixel scaling.
        position /= Utils::getInsideScale();
        position.x = std::round(position.x);
        position.y = std::round(position.y);
        position *= Utils::getInsideScale();

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
                    if (iter->get()->item->isPlayerInRange())
                    {
                        iter->get()->item->onPlayerLeaves(*_session);
                    }
                }
                iter->get()->removePhysics(_physicsWorld);
                _placedItems.erase(iter);
                return;
            }
        }

        std::cout << "Unable to find placeable item to remove it from walkable area: " << id << std::endl;
    }
} // namespace space
