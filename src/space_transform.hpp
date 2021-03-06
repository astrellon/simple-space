#pragma once

#include <SFML/Graphics.hpp>

#include <math.h>

namespace space
{
    class SpaceTransform
    {
        public:
            // Fields
            float rotation;
            float scale;
            sf::Vector2f position;

            // Constructor
            SpaceTransform() : rotation(0), scale(1) { }
            SpaceTransform(float rotation, sf::Vector2f position, float scale) :
                rotation(rotation), scale(scale), position(position) { }

            // Methods
            inline const sf::Transform &getTransform()
            {
                auto angle  = -rotation * 3.141592654f / 180.f;
                auto cosine = static_cast<float>(std::cos(angle));
                auto sine   = static_cast<float>(std::sin(angle));

                auto m = _transform.getMatrix();
                m[0] = scale * cosine;
                m[4] = scale * sine;
                m[12] = position.x;

                m[1] = -m[4];
                m[5] = m[0];
                m[13] = position.y;

                m[10] = scale;

                return _transform;
            }

        private:
            // Fields
            sf::Transform _transform;

            // Methods
    };
} // space