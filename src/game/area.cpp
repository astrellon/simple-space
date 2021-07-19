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
    Area::Area(AreaType type, SpaceObject *partOfObject) : _type(type), _partOfObject(partOfObject), _quadtree(sf::FloatRect(0, 0, 8192, 8192))
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
        _lastDrawnObjects.clear();

        auto &view = target.camera().view();
        auto scale = target.camera().scale();
        auto size = view.getSize() * scale;
        auto viewBox = sf::FloatRect(view.getCenter() * scale - size * 0.5f, size);

        static std::vector<SpaceObject *> inViewObjects;
        inViewObjects.clear();
         _quadtree.query(viewBox, inViewObjects);

         _lastDrawnObjects.insert(_lastDrawnObjects.begin(), inViewObjects.begin(), inViewObjects.end());
         _lastDrawnObjects.insert(_lastDrawnObjects.end(), _notInQuadTree.begin(), _notInQuadTree.end());

        std::sort(_lastDrawnObjects.begin(), _lastDrawnObjects.end(), sortByPosition);

        for (auto drawable : _lastDrawnObjects)
        {
            drawable->draw(session, target);
        }
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
        sf::FloatRect box(mousePosition.x, mousePosition.y, 1, 1);
        auto queryFound = _quadtree.query(box);
        if (queryFound.size() > 0)
        {
            for (auto iter = queryFound.rbegin(); iter != queryFound.rend(); ++iter)
            {
                if ((*iter)->doesMouseHover(inRelationTo, session, mousePosition))
                {
                    session.setNextMouseHover(*iter);
                    return true;
                }
            }
        }

        for (auto iter = _notInQuadTree.rbegin(); iter != _notInQuadTree.rend(); ++iter)
        {
            if ((*iter)->doesMouseHover(inRelationTo, session, mousePosition))
            {
                session.setNextMouseHover(*iter);
                return true;
            }
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

    const std::vector<SpaceObject *> &Area::objects(SpaceObjectType type) const
    {
        auto find = _groupedObjects.find(type);
        if (find == _groupedObjects.end())
        {
            static std::vector<SpaceObject *> empty;
            return empty;
        }

        return find->second;
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

        _groupedObjects[obj->type()].push_back(obj);

        if (_type == AreaType::PlanetSurface && obj->isStaticObject() && obj->getBounds().getSize() != sf::Vector2f())
        {
            _quadtree.add(obj);
        }
        else
        {
            _notInQuadTree.push_back(obj);
        }

        PlacedItem *placedItem;
        if (obj->tryCast(placedItem))
        {
            placedItem->item->onPlaced(*placedItem);
        }
    }
    void Area::removeObject(SpaceObject *obj)
    {
        Utils::remove(_objects, obj);

        auto find = _groupedObjects.find(obj->type());
        if (find != _groupedObjects.end())
        {
            Utils::remove(find->second, obj);
        }

        obj->insideArea(nullptr);

        _quadtree.remove(obj);
        Utils::remove(_notInQuadTree, obj);
    }

    PlacedItem *Area::addPlaceable(GameSession &session, PlaceableItem *item, sf::Vector2f position)
    {
        // Snap position to the inside pixel scaling.
        position /= Utils::InsideScale;
        position.x = std::round(position.x);
        position.y = std::round(position.y);
        position *= Utils::InsideScale;

        auto placedItem = session.createObject<PlacedItem>(item);
        placedItem->transform().position = position;
        placedItem->updateWorldBounds();

        addObject(placedItem);

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

    void Area::getObjectsNearby(sf::FloatRect inRect, std::vector<SpaceObject *> &result) const
    {
        _quadtree.query(inRect, result);

        for (auto obj : _notInQuadTree)
        {
            auto pos = obj->transform().position;
            if (inRect.contains(pos))
            {
                result.push_back(obj);
            }
        }
    }

    void Area::getObjectsNearby(float radius, const SpaceObject &toObject, std::vector<SpaceObject *> &result) const
    {
        // auto pos = Utils::getPosition(toObject.worldTransform());
        auto pos = toObject.transform().position;
        sf::FloatRect box(pos.x - radius, pos.y - radius, radius * 2, radius * 2);
        getObjectsNearby(box, result);
        for (auto iter = result.begin(); iter != result.end(); ++iter)
        {
            if ((*iter)->id == toObject.id)
            {
                result.erase(iter);
                break;
            }
        }
    }

    void Area::getObjectsNearby(float radius, const sf::Vector2f &position, std::vector<SpaceObject *> &result) const
    {
        sf::FloatRect box(position.x - radius, position.y - radius, radius * 2, radius * 2);
        getObjectsNearby(box, result);
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

    bool Area::sortByPosition(SpaceObject *obj1, SpaceObject *obj2)
    {
        auto layerDiff = obj1->drawLayer() - obj2->drawLayer();
        if (layerDiff == 0)
        {
            auto diff = obj1->transform().position.y - obj2->transform().position.y;
            if (diff == 0.0f)
            {
                return obj1->id < obj2->id;
            }
            return diff < 0.0f;
        }
        return layerDiff < 0;
    }
} // space