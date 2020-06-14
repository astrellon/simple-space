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
            sf::Vector2f position;

            // Constructor
            SpaceTransform() : rotation(0) { }

            // Methods
            inline const sf::Transform &getTransform()
            {
                auto angle  = -rotation * 3.141592654f / 180.f;
                auto cosine = static_cast<float>(std::cos(angle));
                auto sine   = static_cast<float>(std::sin(angle));

                auto m = const_cast<float *>(_transform.getMatrix());
                m[0] = cosine;
                m[4] = sine;
                m[12] = position.x;

                m[1] = -sine;
                m[5] = cosine;
                m[13] = position.y;

                return _transform;
            }

        private:
            // Fields
            sf::Transform _transform;

            // Methods
    };
} // space