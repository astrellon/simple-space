#pragma once

#include "camera.hpp"
#include "non_copyable.hpp"

#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;

    class RenderCamera : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            RenderCamera(Engine &engine, std::string debugName);
            ~RenderCamera();

            // Methods
            Camera &camera() { return _camera; }
            const Camera &camera() const { return _camera; }
            sf::RenderTexture &texture() { return _texture; }
            const sf::RenderTexture &texture() const { return _texture; }

            void update(sf::Time dt);
            void onResize(sf::Vector2f area, float cameraScale);
            void preDraw();

            void drawSprite(const sf::Sprite &sprite, const sf::Transform &worldTransform);
            void commitBatch();

            operator Camera &() { return _camera; }
            operator sf::RenderTexture &() { return _texture; }

        private:
            // Fields
            Camera _camera;
            sf::RenderTexture _texture;
            bool _created;
            sf::VertexArray _batchBuffer;
            const sf::Texture *_batchTexture;
            int _batchBufferIndex;

            // Methods
    };
} // space