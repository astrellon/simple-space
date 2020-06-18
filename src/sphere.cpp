#include "sphere.hpp"

#include <math.h>
#include <iostream>

#include "engine.hpp"

namespace space
{
    Sphere::Sphere(float radius, unsigned int rings, unsigned int sections, sf::Shader *shader) :
        _inited(false), _radius(radius), _rings(rings), _sections(sections),
        _vertexBuffer(0), _texcoordBuffer(0), _shader(shader)
    {

    }

    void Sphere::draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (!_inited)
        {
            generateMesh();
            _inited = true;
        }

        sf::Shader::bind(_shader);

        sf::Glsl::Mat4 mat4(parentTransform.getMatrix());
        mat4.array[14] = std::sin(engine.timeSinceStart().asSeconds()) * 50;

        _shader->setUniform("transform", mat4);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // glEnableVertexAttribArray(1);
        // glBindBuffer(GL_ARRAY_BUFFER, _texcoordBuffer);
        // glVertexAttribPointer(
        //     1,        // attribute. No particular reason for 0, but must match the layout in the shader.
        //     2,        // size
        //     GL_FLOAT, // type
        //     GL_FALSE, // normalized?
        //     0,        // stride
        //     (void *)0 // array buffer offset
        // );

        glDisable(GL_DEPTH_TEST);

        glDrawElements(GL_TRIANGLES, _indices.size() / 3, GL_UNSIGNED_SHORT, _indices.data());

        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        sf::Shader::bind(nullptr);
    }

    void Sphere::generateMesh()
    {
        float const R = 1. / (float)(_rings - 1);
        float const S = 1. / (float)(_sections - 1);

        _vertices.resize(_rings * _sections * 3);
        _texcoords.resize(_rings * _sections * 2);
        auto v = _vertices.begin();
        auto t = _texcoords.begin();

        for (auto r = 0; r < _rings; r++)
        for (auto s = 0; s < _sections; s++)
        {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            *t++ = s * S;
            *t++ = r * R;

            *v++ = x * _radius;
            *v++ = y * _radius;
            *v++ = z * _radius;
        }

        _indices.resize(_rings * _sections * 6);
        auto i = _indices.begin();
        for (auto r = 0; r < _rings; r++)
        for (auto s = 0; s < _sections; s++)
        {
            *i++ = r * _sections + s;
            *i++ = r * _sections + (s + 1);
            *i++ = (r + 1) * _sections + (s + 1);

            *i++ = r * _sections + (s + 1);
            *i++ = (r + 1) * _sections + (s + 1);
            *i++ = (r + 1) * _sections + s;
        }

        // _vertices.push_back(-5);
        // _vertices.push_back(0);
        // _vertices.push_back(0);
        // _vertices.push_back(5);
        // _vertices.push_back(0);
        // _vertices.push_back(0);
        // _vertices.push_back(0);
        // _vertices.push_back(5);
        // _vertices.push_back(0);

        // _texcoords.push_back(0);
        // _texcoords.push_back(0);
        // _texcoords.push_back(1);
        // _texcoords.push_back(0);
        // _texcoords.push_back(0);
        // _texcoords.push_back(1);

        for (auto i = 0; i < 9; i += 3)
        {
            std::cout << "Vert: " << _vertices[i] << ", " << _vertices[i + 1] << ", " << _vertices[i + 2] << std::endl;
        }

        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &_texcoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _texcoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, _texcoords.size() * sizeof(GLfloat), _texcoords.data(), GL_STATIC_DRAW);

        // _indices.push_back(0);
        // _indices.push_back(1);
        // _indices.push_back(2);
        // _indices.push_back(3);
        // _indices.push_back(4);
        // _indices.push_back(5);
        // _indices.push_back(6);
        // _indices.push_back(7);
        // _indices.push_back(8);

        // glGenBuffers(1, &_indicesBuffer);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBuffer);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), _indices.data(), GL_STATIC_DRAW);
    }
} // namespace space