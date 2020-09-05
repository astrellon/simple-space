#include "portal_shadow.hpp"

#include <SFML/OpenGL.hpp>

#include <iostream>

namespace space
{
    Polygon PortalShadow::calcShadow() const
    {
        auto dir1 = (point1 - viewPoint).normalised();
        auto dir2 = (point2 - viewPoint).normalised();

        auto endPoint1 = point1 + dir1 * 200.0f;
        auto endPoint2 = point2 + dir2 * 200.0f;

        Polygon result;
        result.push_back(point1);
        result.push_back(endPoint1);
        result.push_back(endPoint2);
        result.push_back(point2);
        return result;
    }
} // space