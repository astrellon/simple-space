#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;

    class Particles
    {
        public:
            // Fields

            // Constructor
            Particles(Engine &engine, int numParticles, GLenum bufferUsage);

            // Methods
            void update(sf::Time dt);
            void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

            virtual void onInit() = 0;
            virtual void onUpdate(sf::Time dt);
            virtual bool onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);
            virtual void onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        protected:
            // Fields
            Engine &_engine;
            int _numParticles;
            GLuint _billboardVertexBuffer;
            GLuint _particlesPositionBuffer;
            GLuint _particlesColourBuffer;
            GLenum _bufferUsage;
            bool _inited;

            std::vector<sf::Vector2f> _positions;
            std::vector<sf::Color> _colours;

            // Methods
            void syncPositionsColours();
    };
} // space