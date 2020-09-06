#include "portal_shadow.hpp"

#include <SFML/OpenGL.hpp>

#include <iostream>

#include "../utils.hpp"

namespace space
{
    PortalShadow::PortalShadow()
    {

    }

    Polygon PortalShadow::calcShadow()
    {
        calcViewLines();

        //auto viewPoint = view.getCenter();
        auto size = view.getSize();
        auto dir1 = (point1 - viewPoint).normalised();
        auto dir2 = (point2 - viewPoint).normalised();

        std::cout << viewPoint << std::endl;

        Polygon result;
        result.push_back(point1);

        sf::Vector2f endPoint1, endPoint2;
        auto index = 0;

        while (!Utils::tryGetIntersection(point1, dir1, _viewLines[index].p1, _viewLines[index].p2, &endPoint1))
        {
            ++index;
        }

        auto origIndex = index;
        while (!Utils::tryGetIntersection(point2, dir2, _viewLines[index].p1, _viewLines[index].p2, &endPoint2))
        {
            ++index;
            if (index >= 4)
            {
                index = 0;
            }

            if (index == origIndex)
            {
                break;
            }
        }

        result.push_back(endPoint1);
        // if (origIndex != index)
        // {
        //     result.push_back(_viewLines[origIndex].p2);
        // }

        // // for (auto i = (origIndex + 1) % 4; i != index; i++)
        // // {
        // //     result.push_back(_viewLines[i].p2);
        // //     if (i >= 4)
        // //     {
        // //         i = -1;
        // //     }
        // // }

        // if (origIndex != index)
        // {
        //     result.push_back(_viewLines[index].p2);
        // }
        result.push_back(endPoint2);
        result.push_back(point2);
        return result;
    }

    void PortalShadow::calcViewLines()
    {
        auto size = view.getSize() * 0.5f;

        sf::Vector2f topLeft(viewPoint.x - size.x, viewPoint.y - size.y);
        sf::Vector2f topRight(viewPoint.x + size.x, viewPoint.y - size.y);
        sf::Vector2f bottomLeft(viewPoint.x - size.x, viewPoint.y + size.y);
        sf::Vector2f bottomRight(viewPoint.x + size.x, viewPoint.y + size.y);

        _viewLines[0].p1 = topLeft;
        _viewLines[0].p2 = topRight;
        _viewLines[1].p1 = topRight;
        _viewLines[1].p2 = bottomRight;
        _viewLines[2].p1 = bottomRight;
        _viewLines[2].p2 = bottomLeft;
        _viewLines[3].p1 = bottomLeft;
        _viewLines[3].p2 = topLeft;
    }
} // space