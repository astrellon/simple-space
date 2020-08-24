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
        _main.sortEveryDraw = true;
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
        _background.draw(session, target);
        _main.draw(session, target);
        _foreground.draw(session, target);
    }

    void WalkableArea::onPostLoad(GameSession &session)
    {
        for (auto &id : _onPostLoadCharacters)
        {
            Character *character;
            if (session.tryGetSpaceObject<Character>(id, &character))
            {
                addCharacter(character);
            }
            else
            {
                std::cout << "Unable to find character '" << id << "' for walkable area" << std::endl;
            }
        }

        _onPostLoadCharacters.clear();

        for (auto &pair : _onPostLoadPlaceables)
        {
            auto itemId = std::get<0>(pair);
            auto position = std::get<1>(pair);

            PlaceableItem *item;
            if (!session.tryGetItem<PlaceableItem>(itemId, &item))
            {
                std::cout << "Unable to find placeable item for placed item: " << itemId << std::endl;
                continue;
            }

            addPlaceable(item, position);
        }

        _onPostLoadPlaceables.clear();
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
        _main.addObject(character);

        character->insideArea(this);
        character->addToPhysicsWorld(&_physicsWorld);
    }
    void WalkableArea::removeCharacter(Character *character)
    {
        Utils::remove(_characters, character);
        _main.removeObject(character);

        character->removeFromPhysicsWorld(&_physicsWorld);
        character->insideArea(nullptr);
    }

    PlacedItem *WalkableArea::addPlaceable(PlaceableItem *item, sf::Vector2f position)
    {
        DrawLayer *layer;
        if (!tryGetLayer(item->placeableDefinition.drawLayer, &layer))
        {
            return nullptr;
        }

        // Snap position to the inside pixel scaling.
        position /= Utils::getInsideScale();
        position.x = std::round(position.x);
        position.y = std::round(position.y);
        position *= Utils::getInsideScale();

        auto &placedItem = _placedItems.emplace_back(std::make_unique<PlacedItem>(item, position, *this, *layer));
        layer->addObject(placedItem.get());
        placedItem->addPhysics(_physicsWorld);

        item->onPlaced(*placedItem);

        return placedItem.get();
    }

    void WalkableArea::removePlaceable(ItemId id)
    {
        for (auto iter = _placedItems.begin(); iter != _placedItems.end(); ++iter)
        {
            auto placed = iter->get();
            if (placed->item->id == id)
            {
                if (_session != nullptr)
                {
                    _session->playerController().removeCanInteractWith(&placed->interactable());
                    if (placed->item->isPlayerInRange())
                    {
                        placed->item->onPlayerLeaves(*_session);
                    }
                }

                placed->onLayer.removeObject(placed);
                placed->removePhysics(_physicsWorld);
                _placedItems.erase(iter);
                return;
            }
        }

        std::cout << "Unable to find placeable item to remove it from walkable area: " << id << std::endl;
    }

    bool WalkableArea::tryGetLayer(DrawLayers::Type layerType, DrawLayer **result)
    {
        if (layerType == DrawLayers::Background)
        {
            *result = &_background;
            return true;
        }
        else if (layerType == DrawLayers::Main)
        {
            *result = &_main;
            return true;
        }
        else if (layerType == DrawLayers::Foreground)
        {
            *result = &_foreground;
            return true;
        }

        return false;
    }
} // namespace space
