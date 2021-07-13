#include "draw_layer.hpp"

#include <algorithm>

#include "space_object.hpp"
#include "../space_transform.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"
#include "../render_camera.hpp"

namespace space
{
    DrawLayer::DrawLayer(bool sortEveryDraw) : sortEveryDraw(sortEveryDraw)
    {

    }

    void DrawLayer::draw(GameSession &session, RenderCamera &target)
    {
        if (sortEveryDraw)
        {
            sortObjects();
        }

        auto &view = target.camera().view();

        for (auto obj : _drawables)
        {
            if (obj->isInView(view))
            {
                obj->draw(session, target);
            }
        }
    }

    void DrawLayer::addObject(SpaceObject *obj)
    {
        _drawables.push_back(obj);
    }

    void DrawLayer::removeObject(SpaceObject *obj)
    {
        Utils::remove(_drawables, obj);
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
