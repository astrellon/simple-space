#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "space_object.hpp"
#include "../definitions/character_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class GameSession;
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
            virtual void prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

            void insideArea(WalkableArea *area) { _insideArea = area; }
            WalkableArea *insideArea() const { return _insideArea; }

            int tileIndex() const { return _tileIndex; }

            void addToPhysicsWorld(b2World *world);
            void removeFromPhysicsWorld(b2World *world);

        private:
            // Fields
            sf::Clock _timeSinceStart;
            float _rotationSpeed;
            int _tileIndex;

            WalkableArea *_insideArea;
            b2Body *_physicsBody;

            // Methods
    };
} // space