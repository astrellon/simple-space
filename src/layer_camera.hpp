#pragma once

#include <SFML/Graphics.hpp>

namespace space
{
    class Camera;

    class LayerCamera
    {
        public:
            // Fields

            // Constructor
            LayerCamera(const Camera &camera, float distanceScale);

            // Methods
            void update(sf::Time dt);

            const Camera &camera() const { return _camera; }
            float distanceScale() const { return _distanceScale; }
            float scale() const { return _scale; }

            const sf::View &view() const { return _view; }

        private:
            // Fields
            const Camera &_camera;
            sf::View _view;
            float _distanceScale;
            float _scale;
            sf::Vector2f _size;

            // Methods
            void size(const sf::Vector2f &size, float scale);
    };
} // space