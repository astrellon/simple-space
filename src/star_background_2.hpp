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
            StarBackground2(Engine &engine, sf::Shader *shader, float area, int numParticles, float distanceScale);

            // Methods
            void onResize(sf::Vector2f size);

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

            void cameraCenter(sf::Vector2f center);
            Camera &camera() { return _camera; }

            float area() const { return _area; }
            float distanceScale() const { return _distanceScale; }
            int numParticles() const { return _numParticles; }

        private:
            // Fields
            Engine &_engine;
            Camera _camera;
            sf::Shader *_shader;
            float _area;
            float _distanceScale;
            float _numParticles;
            std::vector<std::unique_ptr<StarBackgroundChunk2>> _chunks;

            // Methods
            StarBackgroundChunk2 *getChunk(sf::Vector2i pos);
    };

    class StarBackgroundChunk2
    {
        public:
            // Fields

            // Constructor
            StarBackgroundChunk2(StarBackground2 &parent);

            // Methods
            void position(sf::Vector2i position);
            sf::Vector2i position() const { return _position; }

            void draw(sf::RenderTarget &target, sf::RenderStates &states);

            void active(bool active) { _active = active; }
            bool isActive() const { return _active; }

        private:
            // Fields
            StarBackground2 &_parent;
            sf::VertexArray _vertices;
            sf::Vector2i _position;
            bool _active;

            // Methods
    };
} // space