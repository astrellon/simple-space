#pragma once

#include "camera.hpp"
#include "non_copyable.hpp"

#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;
    class TransitionData;

    class RenderCamera : private NonCopyable
    {
        public:
            // Fields
            const TransitionData *transitionData;

            // Constructor
            RenderCamera(Engine &engine);

            // Methods
            Camera &camera() { return _camera; }
            sf::RenderTexture &texture() { return _texture; }

            void update(sf::Time dt);
            void onResize(sf::Vector2f area, float cameraScale);
            void preDraw();

            operator Camera &() { return _camera; }
            operator sf::RenderTexture &() { return _texture; }

        private:
            // Fields
            Camera _camera;
            sf::RenderTexture _texture;
    };
} // space