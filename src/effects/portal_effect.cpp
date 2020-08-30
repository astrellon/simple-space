#include "portal_effect.hpp"

#include <math.h>
#include "../utils.hpp"
#include "../render_camera.hpp"
#include "../engine.hpp"
#include "../resource_manager.hpp"
#include "../game_session.hpp"
#include <SFML/OpenGL.hpp>

namespace space
{
    PortalEffect::PortalEffect(const ObjectId &id, size_t numParticles) : SpaceObject(id), _vertices(sf::Points, numParticles), _shader(nullptr)
    {
        _particles.resize(numParticles);

        for (auto i = 0; i < numParticles; i++)
        {
            auto &particle = _particles[i];
            auto &vertex = _vertices[i];

            auto angle = Utils::degreesToRadians(Utils::randf(0, 360));
            auto dist = Utils::randf(50, 200);

            vertex.position = sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);
            vertex.color = Utils::hsv(Utils::randf(0, 360), 0.75f, 0.75f);

            angle += Utils::randf(0.2, M_PI_4);

            particle.velocity = sf::Vector2f(std::cos(angle), std::sin(angle)) * 10.0f;
        }
    }

    void PortalEffect::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        if (_shader == nullptr)
        {
            session.engine().resourceManager().shader("portalParticle", &_shader);
        }
        auto seconds = dt.asSeconds();
        for (auto i = 0; i < _particles.size(); i++)
        {
            auto &particle = _particles[i];
            auto &vertex = _vertices[i];
            sf::Vector2f acc(vertex.position);
            auto len = std::sqrt(acc.x * acc.x + acc.y * acc.y);
            acc.x /= len;
            acc.y /= len;

            len = -Utils::clamp(1000.0f / len, 0.1f, 1000.0f);
            acc.x *= len;
            acc.y *= len;

            particle.velocity += acc * seconds;
            vertex.position += particle.velocity * seconds;
        }

        updateWorldTransform(parentTransform);
    }

    void PortalEffect::draw(GameSession &session, sf::RenderTarget &target)
    {
        sf::RenderStates states;
        states.shader = _shader;
        states.transform = _worldTransform;

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        target.draw(_vertices, states);

        glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    }
} // namespace space
