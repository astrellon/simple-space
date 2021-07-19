#include "area_quadtree.hpp"

namespace space
{
    AreaQuadtree::AreaQuadtree(sf::FloatRect boxSize) : quadtree::Quadtree<SpaceObject *>(boxSize)
    {

    }
} // space