#include "draw_layer.hpp"

#include <algorithm>

#include "space_object.hpp"
#include "space_transform.hpp"

#include "../utils.hpp"

namespace space
{
    DrawLayer::DrawLayer() : sortEveryDraw(false)
    {

    }

    void DrawLayer::draw(GameSession &session, sf::RenderTarget &target)
    {
        if (sortEveryDraw)
        {
            sortObjects();
        }

        for (auto obj : _drawables)
        {
            obj->draw(session, target);
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
        return obj1->transform().position.y < obj2->transform().position.y;
    }
} // namespace space
