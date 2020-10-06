#include "area.hpp"

#include "space_object.hpp"
#include "ship.hpp"
#include "star_system.hpp"
#include "planet_surface.hpp"

#include "../game_session.hpp"
#include "../physics/polygon_collider.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"
#include "items/placeable_item.hpp"
#include "items/teleporter.hpp"
#include "items/placed_item.hpp"

namespace space
{
    Area::Area(AreaType type, SpaceObject *partOfObject) : _type(type), _partOfObject(partOfObject), _background(false), _main(true), _foreground(false)
    {
        if (type == AreaType::Ship || type == AreaType::PlanetSurface)
        {
            _physicsWorld = std::make_unique<b2World>(b2Vec2(0, 0));
        }

        if (type == AreaType::Ship) { _partOfShip = dynamic_cast<Ship *>(partOfObject); }
        if (type == AreaType::PlanetSurface) { _partOfPlanetSurface = dynamic_cast<PlanetSurface *>(partOfObject); }
        if (type == AreaType::StarSystem) { _partOfStarSystem = dynamic_cast<StarSystem *>(partOfObject); }
    }

    Area::~Area()
    {

    }

    void Area::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        for (auto obj : _objects)
        {
            obj->update(session, dt, parentTransform);
        }
    }

    void Area::draw(GameSession &session, RenderCamera &target)
    {
        _background.draw(session, target);
        _main.draw(session, target);
        _foreground.draw(session, target);
    }

    void Area::onPostLoad(GameSession &session, LoadingContext &context)
    {
        // auto find = context.postLoadWalkableAreaInstances.find(this);
        // if (find != context.postLoadWalkableAreaInstances.end())
        // {
        //     find->second->applyToWalkableArea(*this, session);
        // }
    }

    bool Area::checkForMouse(GameSession &session, sf::Vector2f mousePosition) const
    {
        if (_main.checkForMouse(session, mousePosition))
        {
            return true;
        }

        if (_background.checkForMouse(session, mousePosition))
        {
            return true;
        }

        return false;
    }

    void Area::addStaticCollider(PolygonCollider &collider)
    {
        if (!_physicsWorld.get())
        {
            std::cout << "Cannot add collider to non-physics area" << std::endl;
            return;
        }
        collider.addToWorld(_physicsWorld.get());
    }
    void Area::removeStaticCollider(PolygonCollider &collider)
    {
        if (!_physicsWorld.get())
        {
            std::cout << "Cannot remove collider from non-physics area" << std::endl;
            return;
        }
        collider.removeFromWorld(_physicsWorld.get());
    }

    void Area::addObject(SpaceObject *obj)
    {
        if (obj->insideArea() != nullptr)
        {
            obj->insideArea()->removeObject(obj);
        }

        obj->insideArea(this);
        _objects.push_back(obj);

        DrawLayer *layer;
        if (tryGetLayer(obj->drawLayer(), &layer))
        {
            layer->addObject(obj);
        }
        else
        {
            std::cout << "Unable to find layer for " << obj->id << std::endl;
        }

    }
    void Area::removeObject(SpaceObject *obj)
    {
        Utils::remove(_objects, obj);
        DrawLayer *layer;
        if (tryGetLayer(obj->drawLayer(), &layer))
        {
            layer->addObject(obj);
        }

        obj->insideArea(nullptr);
    }

    PlacedItem *Area::addPlaceable(GameSession &session, PlaceableItem *item, sf::Vector2f position)
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
        _objects.push_back(placedItem);

        if (_physicsWorld)
        {
            placedItem->addPhysics(*_physicsWorld.get());
        }

        item->onPlaced(*placedItem);

        return placedItem;
    }

    std::vector<PlacedItemPair<Teleporter>> Area::findTeleporters() const
    {
        std::vector<PlacedItemPair<Teleporter>> result;
        addTeleporters(result);

        return result;
    }

    void Area::addTeleporters(std::vector<PlacedItemPair<Teleporter>> &result) const
    {
        for (auto obj : _objects)
        {
            if (obj->type() != PlacedItem::SpaceObjectType())
            {
                continue;
            }

            auto placedItem = dynamic_cast<PlacedItem *>(obj);
            if (placedItem->item->type() != Teleporter::ItemType())
            {
                continue;
            }

            auto teleporter = dynamic_cast<Teleporter *>(placedItem->item);
            result.push_back(PlacedItemPair<Teleporter>(placedItem, teleporter));
        }
    }

    bool Area::tryGetLayer(DrawLayers::Type layer, DrawLayer **result)
    {
        if (layer == DrawLayers::Background) { *result = &_background; return true; }
        if (layer == DrawLayers::Main) { *result = &_main; return true; }
        if (layer == DrawLayers::Foreground) { *result = &_foreground; return true; }

        return false;
    }

    void Area::getObjectsNearby(float radius, const sf::Vector2f &position, Area::FindObjectCallback callback) const
    {
        auto lengthSquared = radius * radius;
        for (auto obj : _objects)
        {
            auto dist = (position - obj->transform().position).lengthSquared();
            if (dist <= lengthSquared)
            {
                callback(obj);
            }
        }
    }

    void Area::getObjectsNearby(float radius, const sf::Vector2f &position, std::vector<SpaceObject *> &result) const
    {
        auto lengthSquared = radius * radius;
        for (auto obj : _objects)
        {
            auto dist = (position - obj->transform().position).lengthSquared();
            if (dist <= lengthSquared)
            {
                result.push_back(obj);
            }
        }
    }
} // space