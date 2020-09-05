#pragma once

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../physics/polygon.hpp"

namespace space
{
    class PortalShadow : private NonCopyable
    {
        public:
            // Fields
            sf::Vector2f viewPoint;
            sf::Vector2f point1;
            sf::Vector2f point2;

            // Constructor

            // Methods
            Polygon calcShadow() const;
    };
} // space