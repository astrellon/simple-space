#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "space_object.hpp"
#include "../definitions/character_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;
    class WalkableArea;

    class Character : public SpaceObject
    {
        public:
            // Fields
            const CharacterDefinition &definition;

            // Inputs
            float rotateInput;
            sf::Vector2f moveInput;

            // Constructor
            Character(const ObjectId &id, const CharacterDefinition &definition);

            // Methods
            virtual void prePhysics(Engine &engine, sf::Time dt, const sf::Transform &parentTransform);
            virtual void update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(Engine &engine, sf::RenderTarget &target);

            void insideArea(WalkableArea *area) { _insideArea = area; }
            WalkableArea *insideArea() const { return _insideArea; }

            void addToPhysicsWorld(b2World *world);
            void removeFromPhysicsWorld(b2World *world);

        private:
            // Fields
            sf::Clock _timeSinceStart;
            float _rotationSpeed;

            WalkableArea *_insideArea;
            b2FixtureDef _physicsFixtureDef;
            b2BodyDef _physicsBodyDef;
            b2Body *_physicsBody;

            // Methods
    };
} // space