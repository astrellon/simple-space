#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "camera.hpp"

namespace space
{
    class Engine;
    class StarBackgroundChunk2;

    class StarBackground2
    {
        public:
            // Fields

            // Constructor
            StarBackground2(Engine &engine, float area, int numParticles, float distanceScale);

            // Methods
            void onResize(sf::Vector2f size);
            void draw(sf::RenderTarget &target);

            void setCameraCenter(sf::Vector2f center);
            Camera &camera() { return _camera; }

            float area() const { return _area; }
            float distanceScale() const { return _area; }
            int numParticles() const { return _numParticles; }

        private:
            // Fields
            Camera _camera;
            float _area;
            float _distanceScale;
            float _numParticles;
            std::vector<std::unique_ptr<StarBackgroundChunk2>> _chunks;

            // Methods
    };

    class StarBackgroundChunk2
    {
        public:
            // Fields

            // Constructor
            StarBackgroundChunk2(StarBackground2 &parent);

            // Methods
            void setPosition(sf::Vector2i position);
            void draw(sf::RenderTarget &target);

        private:
            // Fields
            StarBackground2 &_parent;
            sf::VertexArray _vertices;

            // Methods
    };
} // space