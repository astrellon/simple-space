#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "space_object.hpp"
#include "../definitions/ship_definition.hpp"

namespace space
{
    class Engine;

    class Ship : public SpaceObject
    {
        public:
            // Fields
            const ShipDefinition &definition;

            float rotateInput;
            sf::Vector2f moveInput;

            // Constructor
            Ship(const ObjectId &id, const ShipDefinition &definition);

            // Methods

            float rotationSpeed() const { return _rotationSpeed; }
            sf::Vector2f speed() const { return _speed; }

            virtual void update(Engine &engine, sf::Time dt);
            virtual void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

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