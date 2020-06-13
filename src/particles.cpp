#include "particles.hpp"

#include "utils.hpp"
#include "engine.hpp"

static const GLfloat g_vertex_buffer_data[] = {
    // -0.5f, -0.5f,
    // 0.5f, -0.5f,
    // -0.5f, 0.5f,
    // 0.5f, 0.5f,
    -1, -1,
    1, -1,
    -1, 1,
    1, 1,
};

namespace space
{
    Particles::Particles(Engine &engine, int numParticles) : _engine(engine), _numParticles(numParticles), _inited(false)
    {
    }

    void Particles::update(sf::Time dt)
    {
        if (!_inited)
        {
            glGenBuffers(1, &_billboardVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, _billboardVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            glGenBuffers(1, &_particlesPositionBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, _particlesPositionBuffer);
            glBufferData(GL_ARRAY_BUFFER, _numParticles * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

            glGenBuffers(1, &_particlesColourBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, _particlesColourBuffer);
            glBufferData(GL_ARRAY_BUFFER, _numParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

            _particles.resize(_numParticles);
            _positions.resize(_numParticles);
            _colours.resize(_numParticles);

            for (auto &particle : _particles)
            {
                particle.angle = Utils::randf(0, M_PI * 2);
                particle.distance = Utils::randf(50, 500);
            }

            for (auto &position : _positions)
            {
                position.x = Utils::randf(-1000, 1000);
                position.y = Utils::randf() > 0.5 ? Utils::randf(-200, -50) : Utils::randf(50, 200);
            }

            for (auto &colour : _colours)
            {
                colour.r = Utils::randi(127, 255);
                colour.g = Utils::randi(127, 255);
                colour.b = Utils::randi(127, 255);
            }

            _inited = true;
        }

        auto seconds = dt.asSeconds() * 10;

        for (auto i = 0; i < _numParticles; i++)
        {
            auto &particle = _particles[i];
            //particle.angle += seconds;

            auto &position = _positions[i];
            // position.x = std::cos(particle.angle) * particle.distance;
            // position.y = std::sin(particle.angle) * particle.distance;
            position.x += seconds * particle.angle;
            if (position.x > 1000)
            {
                position.x -= 2000;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, _particlesPositionBuffer);
        glBufferData(GL_ARRAY_BUFFER, _numParticles * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _numParticles * sizeof(GLfloat) * 2, _positions.data());

        glBindBuffer(GL_ARRAY_BUFFER, _particlesColourBuffer);
        glBufferData(GL_ARRAY_BUFFER, _numParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _numParticles * sizeof(GLubyte) * 4, _colours.data());
    }

    void Particles::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        sf::Shader *shader;
        if (!_engine.shaderManager().tryGet("particles", &shader))
        {
            std::cout << "Unable to find shader for particle system" << std::endl;
            return;
        }

        sf::Shader::bind(shader);

        auto combinedTransform = _engine.camera().view().getTransform() * parentTransform;

        sf::Glsl::Mat4 mat4(combinedTransform.getMatrix());
        shader->setUniform("transform", mat4);

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

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _numParticles);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        sf::Shader::bind(nullptr);
    }
} // namespace space
