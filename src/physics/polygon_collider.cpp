#include "polygon_collider.hpp"

#include "../earcut.hpp"

#include "../utils.hpp"

namespace space
{
    PolygonCollider::PolygonCollider(b2BodyType bodyType) : _dirty(false), _body(nullptr), _bodyType(bodyType)
    {

    }

    void PolygonCollider::setMainPolygon(const Points &points)
    {
        _polygon.reserve(1);
        //_polygon[0] = points;
        _polygon.insert(_polygon.begin(), points);
        _dirty = true;
    }
    void PolygonCollider::setHole(const Points &points, size_t holeIndex)
    {
        _polygon.reserve(holeIndex + 1);
        _polygon.insert(_polygon.begin() + holeIndex + 1, points);
        _dirty = true;
    }

    void PolygonCollider::debugDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        updateEarcut();

        // Not intended to be performant!
        for (auto i = 0; i < _indices.size(); i += 3)
        {
            auto vertexArray = sf::VertexArray(sf::Triangles, 3);
            for (auto j = 0; j < 3; j++)
            {
                auto index = _indices[i + j];
                Point p;
                size_t k = 0;

                // The index will be into the polygons list as if it were flat.
                // If we had a main polygon with 4 points and a hole with 4 points and the current index was 5
                // then the actual point would be the 2nd [1] of the hole polygon.
                //for (k = 0; k < _polygon.size() && index >= _polygon[k].size(); k++, index -= _polygon[k].size());
                while (index >= _polygon[k].size())
                {
                    index -= _polygon[k].size();
                    k++;
                }

                p = _polygon[k][index];

                vertexArray[j].position = sf::Vector2f(p[0], p[1]);
                vertexArray[j].color = Utils::hsv((float)i / (float)_indices.size() * 360, 1, 1);
                vertexArray[j].color.a = 120;
            }

            target.draw(vertexArray, parentTransform);
        }
    }

    void PolygonCollider::addToWorld(b2World *world)
    {
        updateEarcut();

        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;

        _body = world->CreateBody(&bodyDef);

        for (auto i = 0; i < _indices.size(); i += 3)
        {
            b2PolygonShape shape;
            std::vector<b2Vec2> points;
            for (auto j = 0; j < 3; j++)
            {
                auto index = _indices[i + j];
                Point p;
                size_t k = 0;

                while (index >= _polygon[k].size())
                {
                    index -= _polygon[k].size();
                    k++;
                }

                p = _polygon[k][index];

                points.push_back(b2Vec2(p[0], p[1]));
            }

            shape.Set(points.data(), 3);

            b2FixtureDef fixtureDef;
            fixtureDef.restitution = 0;
            fixtureDef.friction = 0.5f;
            fixtureDef.density = 5.0f;
            fixtureDef.shape = &shape;

            _body->CreateFixture(&fixtureDef);
        }
    }

    void PolygonCollider::removeFromWorld(b2World *world)
    {
        if (_body)
        {
            world->DestroyBody(_body);
            _body = nullptr;
        }
    }

    void PolygonCollider::updateEarcut()
    {
        if (!_dirty)
        {
            return;
        }

        _dirty = false;
        _indices = mapbox::earcut<uint16_t>(_polygon);
    }
} // namespace space
