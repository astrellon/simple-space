#include "particles_simple.hpp"

#include "../render_camera.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"

static const sf::Vector2f QuadOffsets[] = {
    sf::Vector2f(-1, -1),
    sf::Vector2f(1, -1),
    sf::Vector2f(-1, 1),

    sf::Vector2f(1, -1),
    sf::Vector2f(1, 1),
    sf::Vector2f(-1, 1)
};

namespace space
{
    Particle::Particle() : colour(sf::Color::White), angle(0.0f), distance(10.0f)
    {

    }

    void Particle::update(sf::Time dt)
    {
        angle += dt.asSeconds() * 0.1f;
        position = Utils::radianVector(angle, distance);
    }

    const SpaceObjectType ParticlesSimple::TypeValue = SpaceObjectType::ParticlesSimple;

    ParticlesSimple::ParticlesSimple(const ObjectId &id, int numParticles) : SpaceObject(id, TypeValue), _verticies(sf::Triangles, numParticles * 6), _numParticles(numParticles), _particles(numParticles)
    {
        for (auto i = 0; i < numParticles; i++)
        {
            auto &particle = _particles[i];
            particle.angle = Utils::randf(0, M_PI * 2.0f);
            particle.distance = Utils::randf(40.0f, 800.0f);

            particle.colour = Utils::hsv(Utils::randf(0, 360), Utils::randf(0.5, 1.0), Utils::randf(0.5, 1.0));
        }
    }

    ParticlesSimple *ParticlesSimple::cloneParticlesSimple(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<ParticlesSimple>(newId, _numParticles);
        populateCloneFromThis(result, context);
        return result;
    }

    void ParticlesSimple::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);

        for (auto i = 0; i < _numParticles; i++)
        {
            auto &particle = _particles[i];
            particle.update(dt);
        }
    }

    void ParticlesSimple::draw(GameSession &session, RenderCamera &target)
    {
        updateBillboardOffsets(target, 4.0f);

        for (auto i = 0; i < _numParticles; i++)
        {
            auto &particle = _particles[i];

            for (auto j = 0; j < 6; j++)
            {
                auto &vertex = _verticies[j + i * 6];
                auto offset = _billboardOffsets[j];
                vertex.position = particle.position + offset;
                vertex.color = particle.colour;
            }
        }

        target.texture().draw(_verticies, _worldTransform);
    }

    void ParticlesSimple::updateBillboardOffsets(const RenderCamera &target, float scale)
    {
        auto rotation = target.camera().getRotation();
        auto cameraScale = target.camera().scale();

        sf::Transform trans;
        trans.rotate(rotation);
        trans.scale(scale / cameraScale, scale / cameraScale);
        for (auto i = 0; i < 6; i++)
        {
            _billboardOffsets[i] = trans.transformPoint(QuadOffsets[i]);
        }
    }
} // space