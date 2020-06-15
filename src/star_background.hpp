#pragma once

#include <SFML/Graphics.hpp>

#include <map>

#include "particles.hpp"
#include "non_copyable.hpp"

namespace space
{
    class Engine;
    class StarBackgroundChunk;

    class StarBackground : public Particles
    {
        public:
            // Fields

            // Constructor
            StarBackground(Engine &engine, int numParticlesPerChunk, float chunkArea, float distanceScale);

            // Methods
            void onResize(sf::Vector2f viewport);
            virtual void onInit();
            virtual void onUpdate(sf::Time dt);
            virtual bool onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);
            virtual void onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);

            void bindShader(const sf::Transform &parentTransform);
            void unbindShader();

        private:
            friend class StarBackgroundChunk;

            // Fields
            sf::Shader *_shader;
            sf::Vector2f _chunkDims;
            float _distanceScale;
            float _chunkArea;
            int _numParticlesPerChunk;
            int _numChunks;
            int _nextAvailableIndex;
            sf::Vector2f _viewport;
            std::map<std::pair<int, int>, StarBackgroundChunk> _chunks;
            std::vector<int> _availableIndicies;

            // Methods
    };

    class StarBackgroundChunk
    {
        public:
            // Constructor
            StarBackgroundChunk(int indexOffset);

            // Methods
            void updatePosition(StarBackground &parent, sf::Vector2i position);

            inline int indexOffset() const { return _indexOffset; }

        private:
            // Fields
            int _indexOffset;

    };
} // space
