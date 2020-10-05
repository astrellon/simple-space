#include "walkable_area.hpp"

#include "../engine.hpp"
#include "items/placeable_item.hpp"
#include "items/teleporter.hpp"
#include "character.hpp"
#include "../physics/polygon_collider.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"
#include "../effects/grass_effect.hpp"
#include "../serialisers/loading_context.hpp"

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

        for (auto &placedItem : _placedItems)
        {
            placedItem->update(session, dt, _worldTransform);
        }

        for (auto &character : _characters)
        {
            character->prePhysics(session, dt, parentTransform);
        }

        auto physicsSteps = std::min(dt.asSeconds(), 1.0f / 60.0f);
        _physicsWorld.Step(physicsSteps, 4, 2);

        for (auto grassEffect : _grassEffects)
        {
            grassEffect->update(session, dt, parentTransform);
        }

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

    void WalkableArea::onPostLoad(GameSession &session, LoadingContext &context)
    {
        auto find = context.postLoadWalkableAreaInstances.find(this);
        if (find != context.postLoadWalkableAreaInstances.end())
        {
            find->second->applyToWalkableArea(*this, session);
        }
    }

    bool WalkableArea::checkForMouse(GameSession &session, sf::Vector2f mousePosition)
    {
        for (int i = _characters.size() - 1; i >= 0; --i)
        {
            if (_characters[i]->doesMouseHover(session, mousePosition))
            {
                session.setNextMouseHover(_characters[i]);
                return true;
            }
        }

        for (int i = _placedItems.size() - 1; i >= 0; --i)
        {
            if (_placedItems[i]->doesMouseHover(session, mousePosition))
            {
                session.setNextMouseHover(_placedItems[i]);
                return true;
            }
        }

        return false;
    }

    std::vector<PlacedItemPair<Teleporter>> WalkableArea::findTeleporters() const
    {
        std::vector<PlacedItemPair<Teleporter>> result;

        for (auto placedItem : _placedItems)
        {
            auto teleporter = dynamic_cast<Teleporter *>(placedItem->item);
            if (teleporter == nullptr)
            {
                continue;
            }

            result.push_back(PlacedItemPair<Teleporter>(placedItem, teleporter));
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

    void WalkableArea::addGrassEffect(GrassEffect *grassEffect)
    {
        _grassEffects.push_back(grassEffect);
        _background.addObject(grassEffect);
        grassEffect->transform().scale = Utils::getInsideScale();
    }
    void WalkableArea::removeGrassEffect(const ObjectId &id)
    {
        for (auto iter = _grassEffects.begin(); iter != _grassEffects.end(); ++iter)
        {
            if ((*iter)->id == id)
            {
                _grassEffects.erase(iter);
                _background.removeObject(*iter);
                break;
            }
        }
    }

    PlacedItem *WalkableArea::addPlaceable(GameSession &session, PlaceableItem *item, sf::Vector2f position)
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

        auto placedItem = session.createObject<PlacedItem>(item, position, *this, *layer);
        layer->addObject(placedItem);
        _placedItems.push_back(placedItem);
        placedItem->addPhysics(_physicsWorld);

        item->onPlaced(*placedItem);

        return placedItem;
    }

    void WalkableArea::removePlaceable(GameSession &session, ItemId id)
    {
        for (auto iter = _placedItems.begin(); iter != _placedItems.end(); ++iter)
        {
            auto placed = *iter;
            if (placed->item->id == id)
            {
                session.playerController().removeCanInteractWith(&placed->interactable());
                if (placed->item->isPlayerInRange())
                {
                    placed->item->onPlayerLeaves(session);
                }

                placed->onLayer.removeObject(placed);
                placed->removePhysics(_physicsWorld);
                session.removeSpaceObject(placed->id);
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
