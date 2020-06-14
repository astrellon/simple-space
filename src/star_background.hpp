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
            StarBackground(Engine &engine, float chunkSize, int numParticlesPerChunk);

            // Methods
            void update(sf::Time dt);
            void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            Engine &_engine;
            float _chunkSize;
            int _numParticlesPerChunk;

            std::vector<std::unique_ptr<StarBackgroundChunk>> _chunkList;

            // Methods
            StarBackgroundChunk *getChunk(sf::Vector2i pos);

    };
} // space