#include "particles.hpp"

#include "utils.hpp"
#include "engine.hpp"

#include "debug/draw_debug.hpp"

static const GLfloat g_vertex_buffer_data[] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    -0.5f, 0.5f,
    0.5f, 0.5f,
    // -1, -1,
    // 1, -1,
    // -1, 1,
    // 1, 1,
};

namespace space
{
    Particles::Particles(Engine &engine, int numParticles, GLenum bufferUsage) : _engine(engine), _numParticles(numParticles), _inited(false), _bufferUsage(bufferUsage)
    {
    }

    void Particles::update(sf::Time dt)
    {
        if (!_inited && _numParticles > 0)
        {
            glGenBuffers(1, &_billboardVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, _billboardVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            _positions.resize(_numParticles);
            _colours.resize(_numParticles);

            onInit();

            glGenBuffers(1, &_particlesPositionBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, _particlesPositionBuffer);
            glBufferData(GL_ARRAY_BUFFER, _numParticles * 2 * sizeof(GLfloat), _positions.data(), _bufferUsage);

            glGenBuffers(1, &_particlesColourBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, _particlesColourBuffer);
            glBufferData(GL_ARRAY_BUFFER, _numParticles * 4 * sizeof(GLubyte), _colours.data(), _bufferUsage);

            _inited = true;
        }

        onUpdate(dt);
    }

    void Particles::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (!onPreDraw(target, parentTransform))
        {
            return;
        }

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _billboardVertexBuffer);
        glVertexAttribPointer(
            0,        // attribute. No particular reason for 0, but must match the layout in the shader.
            2,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // 2nd attribute buffer : positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _particlesPositionBuffer);
        glVertexAttribPointer(
            1,        // attribute. No particular reason for 1, but must match the layout in the shader.
            2,        // size : x + y + z + size => 4
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        // 3rd attribute buffer : particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, _particlesColourBuffer);
        glVertexAttribPointer(
            2,                // attribute. No particular reason for 1, but must match the layout in the shader.
            4,                // size : r + g + b + a => 4
            GL_UNSIGNED_BYTE, // type
            GL_TRUE,          // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
            0,                // stride
            (void *)0         // array buffer offset
        );

        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad -> 1

        DrawDebug::glDraw++;
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _numParticles);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        onPostDraw(target, parentTransform);
    }

    void Particles::onUpdate(sf::Time dt)
    {

    }

    bool Particles::onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        return true;
    }

    void Particles::onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {

    }

    void Particles::syncPositions()
    {
        glBindBuffer(GL_ARRAY_BUFFER, _particlesPositionBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _numParticles * sizeof(GLfloat) * 2, _positions.data());
    }
    void Particles::syncColours()
    {
        glBindBuffer(GL_ARRAY_BUFFER, _particlesColourBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _numParticles * sizeof(GLubyte) * 4, _colours.data());
    }
    void Particles::syncPositionsColours()
    {
        syncPositions();
        syncColours();
    }
} // namespace space
