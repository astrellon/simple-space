#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "layer_camera.hpp"
#include "definitions/star_background_options.hpp"

namespace space
{
    class Engine;
    class StarBackgroundChunk;
    class StarBackgroundLayer;
    class RenderCamera;

    class StarBackground
    {
        public:
            // Fields

            // Constructor
            StarBackground(Engine &engine, const StarBackgroundOptions &options);

            // Methods
            void update(sf::Time dt);
            void draw(RenderCamera &renderCamera);

            float area() const { return _options.area; }
            int numParticles() const { return _options.numParticles; }
            int numLayers() const { return _options.numLayers; }
            sf::Color backgroundColour() const { return _options.backgroundColour; }
            sf::Shader *shader() const { return _options.shader; }
            Engine &engine() const { return _engine; }

        private:
            // Fields
            Engine &_engine;
            const StarBackgroundOptions &_options;
            std::vector<std::unique_ptr<StarBackgroundLayer>> _layers;

            // Methods
    };

    class StarBackgroundLayer
    {
        public:
            // Fields

            // Constructor
            StarBackgroundLayer(StarBackground &parent, float distanceScale);

            // Methods
            void update(sf::Time dt);
            void draw(RenderCamera &renderCamera);

            void cameraCenter(sf::Vector2f center);
            LayerCamera &camera() { return _camera; }

            float area() const { return _parent.area(); }
            int numParticles() const { return _parent.numParticles(); }

            float distanceScale() const { return _distanceScale; }

        private:
            // Fields
            StarBackground &_parent;
            LayerCamera _camera;
            float _distanceScale;
            std::vector<std::unique_ptr<StarBackgroundChunk>> _chunks;

            // Methods
            StarBackgroundChunk *getChunk(sf::Vector2i pos);
    };

    class StarBackgroundChunk
    {
        public:
            // Fields

            // Constructor
            StarBackgroundChunk(StarBackgroundLayer &parent);

            // Methods
            void position(sf::Vector2i position);
            sf::Vector2i position() const { return _position; }

            void draw(sf::RenderTarget &target, sf::RenderStates &states);

            void active(bool active) { _active = active; }
            bool isActive() const { return _active; }

        private:
            // Fields
            StarBackgroundLayer &_parent;
            sf::VertexArray _vertices;
            sf::Vector2i _position;
            bool _active;

            // Methods
    };
} // space