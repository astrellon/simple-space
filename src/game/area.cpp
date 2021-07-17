#include "area.hpp"

#include <typeinfo>

#include "space_object.hpp"
#include "space_portal.hpp"
#include "ship.hpp"
#include "star_system.hpp"
#include "planet_surface.hpp"
#include "../serialisers/loading_context.hpp"
#include "items/placeable_item.hpp"
#include "items/teleporter.hpp"
#include "items/placed_item.hpp"
#include "teleporter_list.hpp"

#include "../game_session.hpp"
#include "../physics/polygon_collider.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"

namespace space
{
    Area::Area(AreaType type, SpaceObject *partOfObject) : _type(type), _partOfObject(partOfObject), _background(false), _main(true), _foreground(false)
    {
        if (type == AreaType::Ship || type == AreaType::PlanetSurface)
        {
            _physicsWorld = std::make_unique<b2World>(b2Vec2(0, 0));
            _transform = _transform.scale(Utils::InsideScale, Utils::InsideScale);
        }

        if (type == AreaType::Ship) { _partOfShip = dynamic_cast<Ship *>(partOfObject); }
        if (type == AreaType::PlanetSurface) { _partOfPlanetSurface = dynamic_cast<PlanetSurface *>(partOfObject); }
        if (type == AreaType::StarSystem) { _partOfStarSystem = dynamic_cast<StarSystem *>(partOfObject); }
    }

    Area::~Area()
    {

    }

    void Area::cloneInto(const ObjectId &newIdPrefix, Area &targetArea, const CloneContext &context) const
    {
        for (auto spaceObject : _objects)
        {
            auto newObject = spaceObject->deepClone(newIdPrefix, context);
            targetArea.addObject(newObject);
        }
    }

    void Area::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto transform = parentTransform * _transform;

        if (!session.isEditor())
        {
            if (_physicsWorld)
            {
                for (auto obj : _objects)
                {
                    obj->prePhysics(session, dt, transform);
                }

                auto physicsSteps = std::min(dt.asSeconds(), 1.0f / 60.0f);
                _physicsWorld->Step(physicsSteps, 4, 2);
            }
        }

        for (auto obj : _objects)
        {
            obj->update(session, dt, transform);
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
        auto find = context.postLoadAreaInstances.find(this);
        if (find != context.postLoadAreaInstances.end())
        {
            find->second->applyToArea(*this, session);
        }
    }

    bool Area::checkForMouse(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition) const
    {
        if (_main.checkForMouse(inRelationTo, session, mousePosition))
        {
            return true;
        }

        if (_background.checkForMouse(inRelationTo, session, mousePosition))
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

        auto scale = 1.0f;
        if (_type == AreaType::Ship || _type == AreaType::PlanetSurface)
        {
            scale = 1.0f / Utils::InsideScale;
        }

        collider.addToWorld(_physicsWorld.get(), scale);
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
        if (obj == nullptr)
        {
            throw std::runtime_error("Cannot add null object to area");
        }

        if (obj->insideArea() != nullptr)
        {
            obj->insideArea()->removeObject(obj);
        }

        obj->insideArea(this);
        _objects.push_back(obj);

        SpacePortal *spacePortal;
        if (obj->tryCast(spacePortal))
        {
            _spacePortals.push_back(spacePortal);
        }

        PlacedItem *placedItem;
        if (obj->tryCast(placedItem))
        {
            placedItem->item->onPlaced(*placedItem);
        }

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

        SpacePortal *spacePortal;
        if (obj->tryCast(spacePortal))
        {
            Utils::remove(_spacePortals, spacePortal);
        }

        DrawLayer *layer;
        if (tryGetLayer(obj->drawLayer(), &layer))
        {
            layer->removeObject(obj);
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
        position /= Utils::InsideScale;
        position.x = std::round(position.x);
        position.y = std::round(position.y);
        position *= Utils::InsideScale;

        auto placedItem = session.createObject<PlacedItem>(item);
        placedItem->transform().position = position;
        placedItem->insideArea(this);
        layer->addObject(placedItem);
        _objects.push_back(placedItem);

        item->onPlaced(*placedItem);

        return placedItem;
    }

    void Area::addTeleporters(TeleporterList &result) const
    {
        for (auto obj : _objects)
        {
            PlacedItem *placedItem;
            Teleporter *teleporter;
            if (!obj->tryCast(placedItem) || !placedItem->item->tryCast<Teleporter>(teleporter))
            {
                continue;
            }

            result.addTeleporter(PlacedItemPair<Teleporter>(placedItem, teleporter));
        }
    }

    bool Area::tryGetLayer(DrawLayers::Type layer, DrawLayer **result)
    {
        if (layer == DrawLayers::Background) { *result = &_background; return true; }
        if (layer == DrawLayers::Main) { *result = &_main; return true; }
        if (layer == DrawLayers::Foreground) { *result = &_foreground; return true; }

        return false;
    }

    void Area::getObjectsNearby(float radius, const SpaceObject &toObject, std::vector<SpaceObject *> &result) const
    {
        auto pos = Utils::getPosition(toObject.worldTransform());

        auto lengthSquared = radius * radius;
        for (auto obj : _objects)
        {
            if (obj == &toObject)
            {
                continue;
            }

            auto dist = (pos - obj->transform().position).lengthSquared();
            if (dist <= lengthSquared)
            {
                result.push_back(obj);
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

    bool Area::loopOver(LoopObjectCallback callback)
    {
        for (auto obj : _objects)
        {
            if (!obj->loopOver(callback))
            {
                return false;
            }
        }

        return true;
    }
} // space