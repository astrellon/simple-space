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
            StarBackground(Engine &engine, float chunkSize, int numParticlesPerChunk, float distanceScale);

            // Methods
            void update(sf::Time dt);
            void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            friend class StarBackgroundChunk;

            // Fields
            Engine &_engine;
            sf::Shader *_shader;
            bool _inited;
            float _chunkSize;
            int _numParticlesPerChunk;
            float _distanceScale;

            std::vector<std::unique_ptr<StarBackgroundChunk>> _chunkList;

            // Methods
            StarBackgroundChunk *getChunk(sf::Vector2i pos);
            void init();

    };
} // space