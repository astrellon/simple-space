#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "space_object.hpp"
#include "walkable_area.hpp"
#include "../definitions/ship_definition.hpp"

namespace space
{
    class Engine;

    class Ship : public SpaceObject
    {
        public:
            // Fields
            const ShipDefinition &definition;

            // Inputs
            float rotateInput;
            sf::Vector2f moveInput;

            // Constructor
            Ship(const ObjectId &id, const ShipDefinition &definition);

            // Methods
            float rotationSpeed() const { return _rotationSpeed; }
            sf::Vector2f speed() const { return _speed; }

            WalkableArea &walkableArea() { return _walkableArea; }
            const WalkableArea &walkableArea() const { return _walkableArea; }

            virtual void update(Engine &engine, sf::Time dt);
            virtual void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            WalkableArea _walkableArea;

            // Location
            sf::Sprite _sprite;

            // Current speeds
            float _rotationSpeed;
            sf::Vector2f _speed;

            // Methods
    };
} //