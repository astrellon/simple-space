#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

namespace space
{
    struct Particle
    {
        float angle;
        float distance;
    };

    class Engine;

    class Particles
    {
        public:
            // Fields

            // Constructor
            Particles(Engine &engine, int numParticles);

            // Methods
            void update(sf::Time dt);
            void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            Engine &_engine;
            int _numParticles;
            GLuint _billboardVertexBuffer;
            GLuint _particlesPositionBuffer;
            GLuint _particlesColourBuffer;
            bool _inited;

            std::vector<Particle> _particles;
            std::vector<sf::Vector2f> _positions;
            std::vector<sf::Color> _colours;

            // Methods
    };
} // space