#pragma once

#include <vector>
#include <array>

#include <SFML/Graphics.hpp>
#include "./space_object.hpp"

namespace space
{
    class GameSession;
    class RenderCamera;

    struct Particle
    {
        // Fields
        sf::Vector2f position;
        sf::Color colour;

        float angle;
        float distance;

        // Constructor
        Particle();

        // Methods
        void update(sf::Time dt);
    };

    class ParticlesSimple : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            ParticlesSimple(const ObjectId &id, int numParticles);

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneParticlesSimple(newId, context); }
            ParticlesSimple *cloneParticlesSimple(const ObjectId &newId, const CloneContext &context);

            static const std::string SpaceObjectType() { return "particles"; }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);

        private:
            // Fields
            sf::VertexArray _verticies;
            int _numParticles;
            std::vector<Particle> _particles;
            std::array<sf::Vector2f, 6> _billboardOffsets;

            // Methods
            void updateBillboardOffsets(const RenderCamera &target, float scale);
    };
} // space