#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

using Points = std::vector<sf::Vector2f>;
using Polygon = std::vector<Points>;

namespace space
{
    class PolygonCollider
    {
        public:
            // Fields

            // Constructor
            PolygonCollider(b2BodyType bodyType);

            // Methods
            void setMainPolygon(const Points &points);
            void setHole(const Points &points, size_t holeIndex);

            void addToWorld(b2World *world);
            void removeFromWorld(b2World *world);

            void debugDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        protected:
            // Fields
            Polygon _polygon;
            std::vector<uint16_t> _indices;
            b2BodyType _bodyType;
            b2Body *_body;
            bool _dirty;

            // Methods
            void updateEarcut();
    };
} // space