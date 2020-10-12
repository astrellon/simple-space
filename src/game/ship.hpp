#pragma once

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "space_object.hpp"
#include "area.hpp"
#include "../definitions/ship_definition.hpp"
#include "../effects/engine_flame_effect.hpp"

namespace space
{
    class GameSession;
    class PolygonCollider;
    class RenderCamera;

    class Ship : public SpaceObject
    {
        public:
            // Fields
            const ShipDefinition &definition;

            // Inputs
            float rotateInput;
            sf::Vector2f moveInput;
            bool disableRender;

            // Constructor
            Ship(const ObjectId &id, const ShipDefinition &definition);

            // Methods
            static const std::string SpaceObjectType() { return ShipDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            float rotationSpeed() const { return _rotationSpeed; }
            void rotationSpeed(float rotationSpeed) { _rotationSpeed = rotationSpeed; }

            sf::Vector2f speed() const { return _speed; }
            void speed(const sf::Vector2f &speed) { _speed = speed; }

            sf::Vector2f prevPosition() const { return _prevPosition; }
            void prevPosition(const sf::Vector2f &pos) { _prevPosition = pos; }

            Area &area() { return _area; }
            const Area &area() const { return _area; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const;

            virtual void drawInterior(GameSession &session, RenderCamera &target);

        private:
            // Fields
            Area _area;
            std::vector<std::unique_ptr<PolygonCollider>> _colliders;
            std::vector<std::unique_ptr<EngineFlameEffect>> _engineEffects;

            // Location
            sf::Sprite _sprite;
            sf::FloatRect _spriteBounds;
            sf::Sprite _interiorSprite;
            sf::Vector2f _prevPosition;

            // Current speeds
            float _rotationSpeed;
            sf::Vector2f _speed;

            // Methods
            void createMainCollider();
            void createExtraCollider(const Polygon &polygon);
    };
} //