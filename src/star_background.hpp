#pragma once

#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "star_background_chunk.hpp"

namespace space
{
    class Engine;

    class StarBackground
    {
        public:
            // Fields

            // Constructor
            StarBackground(Engine &engine, const sf::Image &starColour, float chunkSize, int numParticlesPerChunk, float distanceScale);

            // Methods
            void update(sf::Time dt);
            void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            Engine &_engine;
            const sf::Image &_starColour;
            float _chunkSize;
            int _numParticlesPerChunk;
            float _distanceScale;

            std::vector<std::unique_ptr<StarBackgroundChunk>> _chunkList;

            // Methods
            StarBackgroundChunk *getChunk(sf::Vector2i pos);

    };
} // space