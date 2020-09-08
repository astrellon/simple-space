#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../physics/polygon.hpp"
#include "../line.hpp"

namespace space
{
    class PortalShadow : private NonCopyable
    {
        public:
            // Fields
            sf::View view;
            sf::Vector2f viewPoint;
            sf::Vector2f point1;
            sf::Vector2f point2;

            // Constructor
            PortalShadow();

            // Methods
            void calcShadow(Polygon &result, std::array<FloatLine, 2> &outlines);

        private:
            // Fields
            std::array<FloatLine, 4> _viewLines;

            // Methods
            void calcViewLines();
    };
} // space