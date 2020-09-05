#pragma once

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "space_object.hpp"
#include "walkable_area.hpp"
#include "../definitions/ship_definition.hpp"
#include "../effects/engine_flame_effect.hpp"

namespace space
{
    class GameSession;
    class PolygonCollider;

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
            Ship(const ObjectId &id, const ShipDefinition &definition, std::unique_ptr<WalkableArea> walkableArea);

            // Methods
            static const std::string SpaceObjectType() { return ShipDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            float rotationSpeed() const { return _rotationSpeed; }
            void rotationSpeed(float rotationSpeed) { _rotationSpeed = rotationSpeed; }

            sf::Vector2f speed() const { return _speed; }
            void speed(const sf::Vector2f &speed) { _speed = speed; }

            WalkableArea &walkableArea() { return *_walkableArea.get(); }
            const WalkableArea &walkableArea() const { return *_walkableArea.get(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);
            virtual void onPostLoad(GameSession &session);

        private:
            // Fields
            std::unique_ptr<WalkableArea> _walkableArea;
            std::vector<std::unique_ptr<PolygonCollider>> _colliders;
            std::vector<std::unique_ptr<EngineFlameEffect>> _engineEffects;

            // Location
            sf::Sprite _sprite;
            sf::Sprite _interiorSprite;

            // Current speeds
            float _rotationSpeed;
            sf::Vector2f _speed;

            // Methods
            void createMainCollider();
            void createExtraCollider(const Polygon &polygon);
    };
} //