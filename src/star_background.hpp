#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "definitions/star_background_options.hpp"
#include "effects/overlay.hpp"

namespace space
{
    class Engine;
    class StarBackgroundChunk;
    class RenderCamera;

    class StarBackground
    {
        public:
            // Fields

            // Constructor
            StarBackground(Engine &engine, const StarBackgroundOptions &options);

            // Methods
            void draw(RenderCamera &renderCamera);

            float area() const { return _options.area; }
            int numParticles() const { return _options.numParticles; }
            sf::Color backgroundColour() const { return _options.backgroundColour; }
            sf::Shader *shader() const { return _options.shader; }
            Engine &engine() const { return _engine; }

            const StarBackgroundOptions &options() const { return _options; }

        private:
            // Fields
            Engine &_engine;
            const StarBackgroundOptions &_options;
            std::vector<std::unique_ptr<StarBackgroundChunk>> _chunks;
            Overlay _backgroundColour;

            // Methods
            void updateChunksFromCamera(RenderCamera &renderCamera);
            StarBackgroundChunk *getChunk(sf::Vector2i pos);
    };

    class StarBackgroundChunk
    {
        public:
            // Fields

            // Constructor
            StarBackgroundChunk(StarBackground &parent);

            // Methods
            void position(sf::Vector2i position);
            sf::Vector2i position() const { return _position; }

            void draw(RenderCamera &camera, sf::RenderStates &states);

            void active(bool active) { _active = active; }
            bool isActive() const { return _active; }

        private:
            // Fields
            StarBackground &_parent;
            sf::Vertex3dBuffer _vertexBuffer;
            std::vector<sf::Vertex3d> _verticies;
            sf::Vector2i _position;
            bool _active;

            // Methods
    };
} // space