#include "portal_shadow.hpp"

#include <SFML/OpenGL.hpp>

#include <iostream>

#include "../utils.hpp"
#include "../circular_index.hpp"

namespace space
{
    PortalShadow::PortalShadow()
    {

    }

    void PortalShadow::calcShadow(Polygon &result, std::array<FloatLine, 2> &outlines)
    {
        calcViewLines();

        auto dir1 = (point1 - viewPoint).normalised();
        auto dir2 = (point2 - viewPoint).normalised();

        result.push_back(point1);

        sf::Vector2f endPoint1, endPoint2;
        CircularIndex index(_viewLines.size());

        auto checkCounter = 0u;
        // Find intersecting ray for point1
        while (!Utils::tryGetIntersection(point1, dir1, _viewLines[index].point1, _viewLines[index].point2, &endPoint1))
        {
            ++index;
            ++checkCounter;

            if (checkCounter > _viewLines.size())
            {
                std::cout << "Unable to find intersection with screen edge" << std::endl;
                return;
            }
        }

        // Find intersecting ray for point2
        checkCounter = 0u;
        auto origIndex = index;
        while (!Utils::tryGetIntersection(point2, dir2, _viewLines[index].point1, _viewLines[index].point2, &endPoint2))
        {
            ++index;

            if (index == origIndex) { break; }

            ++checkCounter;

            if (checkCounter > _viewLines.size())
            {
                std::cout << "Unable to find intersection with screen edge" << std::endl;
                return;
            }
        }

        result.push_back(endPoint1);

        // 'Walk' around the outside of the screen
        auto walkIndex = origIndex;
        // Check if we need to make a line from the first screen end point to the next corner
        if (origIndex != index)
        {
            result.push_back(_viewLines[walkIndex].point2);
            ++walkIndex;
        }

        // Check if we need to fill in full screen edges
        for (; walkIndex != index; ++walkIndex)
        {
            result.push_back(_viewLines[walkIndex].point2);
        }

        result.push_back(endPoint2);
        result.push_back(point2);

        outlines[0].point1 = point1;
        outlines[0].point2 = endPoint1;
        outlines[1].point1 = point2;
        outlines[1].point2 = endPoint2;
    }

    void PortalShadow::calcViewLines()
    {
        auto size = view.getSize() * 0.5f;

        sf::Vector2f topLeft(viewPoint.x - size.x, viewPoint.y - size.y);
        sf::Vector2f topRight(viewPoint.x + size.x, viewPoint.y - size.y);
        sf::Vector2f bottomLeft(viewPoint.x - size.x, viewPoint.y + size.y);
        sf::Vector2f bottomRight(viewPoint.x + size.x, viewPoint.y + size.y);

        _viewLines[0].point1 = topLeft;
        _viewLines[0].point2 = topRight;
        _viewLines[1].point1 = topRight;
        _viewLines[1].point2 = bottomRight;
        _viewLines[2].point1 = bottomRight;
        _viewLines[2].point2 = bottomLeft;
        _viewLines[3].point1 = bottomLeft;
        _viewLines[3].point2 = topLeft;
    }
} // space