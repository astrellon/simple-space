#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "space_object.hpp"
#include "walkable_area.hpp"
#include "../definitions/ship_definition.hpp"

namespace space
{
    class GameSession;
    class PolygonCollider;

    template <typename T>
    class PlacedItemPair;
    class Teleporter;

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

            std::vector<PlacedItemPair<Teleporter>> findTeleporters() const;

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

        private:
            // Fields
            WalkableArea _walkableArea;
            std::unique_ptr<PolygonCollider> _collider;

            // Location
            sf::Sprite _sprite;
            sf::Sprite _interiorSprite;

            // Current speeds
            float _rotationSpeed;
            sf::Vector2f _speed;

            // Methods
    };
} //