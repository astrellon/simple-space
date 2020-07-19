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
            LayerCamera(float distanceScale);

            // Methods
            void update(sf::Time dt);
            void preDraw(const Camera &camera);

            float distanceScale() const { return _distanceScale; }
            float scale() const { return _scale; }

            const sf::View &view() const { return _view; }

        private:
            // Fields
            sf::View _view;
            float _distanceScale;
            float _scale;
            sf::Vector2f _size;

            // Methods
            void size(const sf::Vector2f &size, float scale);
    };
} // space