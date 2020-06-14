#pragma once

#include <SFML/Graphics.hpp>

#include "particles.hpp"

namespace space
{
    class Engine;

    class StarBackground : public Particles
    {
        public:
            // Fields

            // Constructor
            StarBackground(Engine &engine, int numParticles);

            // Methods
            virtual void onInit();
            virtual void onUpdate(sf::Time dt);
            virtual bool onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);
            virtual void onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);


        private:
            // Fields
            sf::Shader *_shader;
            sf::Vector2f _prevCameraCenter;

            // Methods
    };
} // space