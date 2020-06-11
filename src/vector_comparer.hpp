#pragma once

#include <SFML/System.hpp>

namespace space
{
    struct Vector2fComparator
    {
        bool operator() (sf::Vector2f lhs, sf::Vector2f rhs) const
        {
            return lhs.x < lhs.y || rhs.x < rhs.y;
        }
    };

    struct Vector2iComparator
    {
        bool operator() (sf::Vector2i lhs, sf::Vector2i rhs) const
        {
            return lhs.x < rhs.x || lhs.y < rhs.y;
        }
    };
}