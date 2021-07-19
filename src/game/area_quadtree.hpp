#pragma once

#include <SFML/Graphics.hpp>

#include "../quadtree/Quadtree.h"
#include "../non_copyable.hpp"

namespace space
{
    class SpaceObject;

    class AreaQuadtree : public quadtree::Quadtree<SpaceObject *>, private NonCopyable
    {
        public:
            // Fields

            // Constructor
            AreaQuadtree(sf::FloatRect boxSize);

            // Methods
    };
} // space