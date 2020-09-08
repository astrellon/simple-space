#pragma once

#include <SFML/System.hpp>

namespace space
{
    template <typename T>
    class Line
    {
        public:
            // Fields
            sf::Vector2<T> point1;
            sf::Vector2<T> point2;

            // Constructor
            Line();
            Line(const sf::Vector2<T> &point1, const sf::Vector2<T> &point2);

            // Methods
    };

    #include "line.inl"

    typedef Line<float> FloatLine;
    typedef Line<int> IntLine;
} // space