#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace space
{
    class Engine;

    class ParticleSystem
    {
        public:
            typedef std::vector<sf::Vector2f> Vectors;
            typedef std::vector<sf::Time> Times;
            typedef std::vector<bool> Booleans;

            ParticleSystem();

            std::size_t numAwake() const;

            std::size_t maxNumberOfParticles() const;
            void maxNumberOfParticles(std::size_t number);

            //void texture(sf::Texture &texture);
            sf::Sprite *sprite();

            void wakeup(std::size_t number);

            void update(sf::Time dt);
            void draw(Engine *engine, sf::RenderTarget &target);

        private:
            Vectors _positions;
            Vectors _velocity;
            Times _ages;
            sf::Sprite _sprite;

            std::size_t _numAwake;
            std::size_t _maxNumberOfParticles;

            void swap(std::size_t index1, std::size_t index2);
            void kill(std::size_t index);
            void restart(std::size_t index);
    };
} // town

