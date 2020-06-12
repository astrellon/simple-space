#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "space_object.hpp"
#include "../definitions/ship_definition.hpp"

namespace space
{
    class Ship : public SpaceObject
    {
        public:
            // Fields
            const ShipDefinition &definition;

            // Constructor
            Ship(const ObjectId &id, const ShipDefinition &definition);

            // Methods

            float rotationSpeed() const { return _rotationSpeed; }
            sf::Vector2f speed() const { return _speed; }

            virtual void update(sf::Time dt);
            virtual void draw(sf::RenderTarget &target);

        private:
            // Fields

            // Location
            sf::Sprite _sprite;

            // Current speeds
            float _rotationSpeed;
            sf::Vector2f _speed;

            // Methods
    };
} //