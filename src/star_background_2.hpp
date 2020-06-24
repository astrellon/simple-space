#pragma once

#include <SFML/Graphics.hpp>

#include "camera.hpp"

namespace space
{
    class Engine;

    class StarBackground2
    {
        public:
            // Fields

            // Constructor
            StarBackground2(Engine &engine, float area, int numParticles, float distanceScale, sf::Vector2i position);

            // Methods
            void draw(sf::RenderTarget &target);

            void setCameraCenter(sf::Vector2f center);
            Camera &camera() { return _camera; }

            float area() const { return _area; }
            float distanceScale() const { return _area; }

        private:
            // Fields
            sf::VertexArray _vertices;
            Camera _camera;
            float _area;
            float _distanceScale;

            // Methods
    };
} // space