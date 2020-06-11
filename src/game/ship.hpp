#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../non_copyable.hpp"

namespace space
{
    class Ship : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            Ship(const sf::Texture *texture, float maxRotation, float maxSpeed): _texture(texture), _maxRotation(maxRotation), _maxSpeed(maxSpeed)
            {

            }

            // Methods
            sf::Transform transform() const { return _transform; }

            float rotationSpeed() const { return _rotationSpeed; }
            sf::Vector2f speed() const { return _speed; }

            float maxRotation() const { return _maxRotation; }
            float maxSpeed() const { return _maxSpeed; }

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

        private:
            // Fields
            const sf::Texture *_texture;

            // Location
            sf::Transform _transform;

            // Current speeds
            float _rotationSpeed;
            sf::Vector2f _speed;

            // Limits
            float _maxRotation;
            float _maxSpeed;

            // Methods
    };
} //