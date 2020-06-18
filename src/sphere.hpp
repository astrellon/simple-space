#pragma once

#include <vector>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;

    class Sphere
    {
        public:
            // Fields

            // Constructor
            Sphere(float radius, unsigned int rings, unsigned int sections, sf::Shader *shader);

            // Methods
            void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            sf::Shader *_shader;
            bool _inited;
            float _radius;
            unsigned int _rings;
            unsigned int _sections;
            GLuint _vertexBuffer;
            GLuint _texcoordBuffer;
            std::vector<GLfloat> _vertices;
            std::vector<GLfloat> _texcoords;
            std::vector<GLushort> _indices;

            // Methods
            void generateMesh();
    };
} // space