#pragma once

#include "../game/space_object.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;

    class PortalEffect : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            PortalEffect(const ObjectId &id, size_t numParticles);

            // Methods
            void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            void draw(GameSession &session, sf::RenderTarget &target);

            static const std::string SpaceObjectType() { return "portal-effect"; }
            virtual std::string type() const { return SpaceObjectType(); }

        private:
            struct Particle
            {
                sf::Vector2f velocity;
            };

            // Fields
            sf::VertexArray _vertices;
            std::vector<Particle> _particles;

            // Methods
    };
} // space