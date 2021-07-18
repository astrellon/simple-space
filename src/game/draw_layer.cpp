#include "draw_layer.hpp"

#include <algorithm>

#include "space_object.hpp"
#include "items/placed_item.hpp"

#include "../space_transform.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"
#include "../render_camera.hpp"

namespace space
{
    DrawLayerPlacedItemsTree::DrawLayerPlacedItemsTree(sf::Vector2f boxSize) :
        quadtree::Quadtree<PlacedItem *>(sf::FloatRect(0, 0, boxSize.x, boxSize.y))
    {

    }

    DrawLayer::DrawLayer(bool useQuadTree, bool sortEveryDraw) : sortEveryDraw(sortEveryDraw), useQuadTree(useQuadTree), _quadtree(sf::Vector2f(4096 * 2, 4096 * 2))
    {

    }

    void DrawLayer::draw(GameSession &session, RenderCamera &target)
    {
        std::vector<PlacedItem *> inView;

        if (useQuadTree)
        {
            auto &view = target.camera().view();
            auto scale = target.camera().scale();
            auto size = view.getSize() * scale;
            auto viewBox = sf::FloatRect(view.getCenter() * scale - size * 0.5f, size);
            inView = _quadtree.query(viewBox);
        }

        if (sortEveryDraw)
        {
            // sortObjects();
            static std::vector<SpaceObject *> objs;
            auto numObjs = inView.size() + _drawables.size();
            objs.clear();
            objs.reserve(numObjs);

            objs.insert(objs.begin(), inView.begin(), inView.end());
            objs.insert(objs.end(), _drawables.begin(), _drawables.end());

            std::sort(objs.begin(), objs.end(), sortByPosition);

            for (auto obj : objs)
            {
                obj->draw(session, target);
            }
        }
        else
        {
            for (auto obj : inView)
            {
                obj->draw(session, target);
            }
            for (auto obj : _drawables)
            {
                obj->draw(session, target);
            }
        }
    }

    void DrawLayer::addObject(SpaceObject *obj)
    {
        PlacedItem *placedItem;
        if (useQuadTree && obj->tryCast(placedItem))
        {
            _quadtree.add(placedItem);
        }
        else
        {
            _drawables.push_back(obj);
        }
    }

    void DrawLayer::removeObject(SpaceObject *obj)
    {
        PlacedItem *placedItem;
        if (useQuadTree && obj->tryCast(placedItem))
        {
            _quadtree.remove(placedItem);
        }
        else
        {
            Utils::remove(_drawables, obj);
        }
    }

    void DrawLayer::sortObjects()
    {
        std::sort(_drawables.begin(), _drawables.end(), sortByPosition);
    }

    bool DrawLayer::sortByPosition(SpaceObject *obj1, SpaceObject *obj2)
    {
        auto diff = obj1->transform().position.y - obj2->transform().position.y;
        if (diff == 0.0f)
        {
            return obj1->id < obj2->id;
        }
        return diff < 0.0f;
    }

    bool DrawLayer::checkForMouse(const Area *inRelationToArea, GameSession &session, sf::Vector2f mousePosition) const
    {
        auto mouseBox = sf::FloatRect(mousePosition, sf::Vector2f(1, 1));

        auto quadResult = _quadtree.query(mouseBox);
        if (quadResult.size() > 0)
        {
            std::cout << "Quad tree over: " << quadResult.size() << std::endl;
        }

        for (auto iter = _drawables.rbegin(); iter != _drawables.rend(); ++iter)
        {
            if ((*iter)->doesMouseHover(inRelationToArea, session, mousePosition))
            {
                session.setNextMouseHover(*iter);
                return true;
            }
        }

        return false;
    }
} // namespace space
