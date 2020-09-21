#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "space_object.hpp"
#include "../definitions/character_definition.hpp"
#include "../animated_sprite.hpp"

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
            static const std::string SpaceObjectType() { return CharacterDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

            void insideArea(WalkableArea *area) { _insideArea = area; }
            WalkableArea *insideArea() const { return _insideArea; }

            void flipSprite(bool flip) { _flipSprite = flip; }
            bool flipSprite() const { return _flipSprite; }

            AnimatedSprite &sprite() { return _sprite; }
            const AnimatedSprite &sprite() const { return _sprite; }

            b2Body *physicsBody() { return _physicsBody; }

            void addToPhysicsWorld(b2World *world);
            void removeFromPhysicsWorld(b2World *world);

        protected:
            // Fields
            WalkableArea *_insideArea;
            b2Body *_physicsBody;
            AnimatedSprite _sprite;
            bool _flipSprite;

            // Methods
    };
} // space