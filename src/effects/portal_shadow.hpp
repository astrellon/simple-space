#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../physics/polygon.hpp"

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
            Polygon calcShadow();

        private:
            struct Line
            {
                sf::Vector2f p1;
                sf::Vector2f p2;

                Line() { }
                Line(const sf::Vector2f &p1, const sf::Vector2f &p2) : p1(p1), p2(p2) { }
            };

            // Fields
            std::array<Line, 4> _viewLines;

            // Methods
            void calcViewLines();
    };
} // space