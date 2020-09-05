#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "polygon.hpp"

namespace space
{
    class PolygonCollider
    {
        public:
            // Fields

            // Constructor
            PolygonCollider(b2BodyType bodyType);

            // Methods
            void setMainPolygon(const Polygon &points);
            void setHole(const Polygon &points, size_t holeIndex);

            void addToWorld(b2World *world);
            void removeFromWorld(b2World *world);

            void debugDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        protected:
            // Fields
            std::vector<Polygon> _polygons;
            std::vector<uint16_t> _indices;
            b2BodyType _bodyType;
            b2Body *_body;
            bool _dirty;

            // Methods
            void updateEarcut();
    };
} // space