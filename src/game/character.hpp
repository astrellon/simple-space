#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "space_object.hpp"
#include "../definitions/character_definition.hpp"
#include "../animated_sprite.hpp"

namespace space
{
    class GameSession;
    class RenderCamera;

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
            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneCharacter(newId, context); }
            Character *cloneCharacter(const ObjectId &newId, const CloneContext &context);

            static const std::string SpaceObjectType() { return CharacterDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual bool doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const;

            void flipSprite(bool flip) { _flipSprite = flip; }
            bool flipSprite() const { return _flipSprite; }

            AnimatedSprite &sprite() { return _sprite; }
            const AnimatedSprite &sprite() const { return _sprite; }

            b2Body *physicsBody() { return _physicsBody; }

            virtual void insideArea(Area *area);
            virtual Area *insideArea() const { return _insideArea; }

            virtual const CompendiumDefinition *compendiumDefinition() const { return definition.compendiumDef; }

            void addToPhysicsWorld(b2World *world);
            void removeFromPhysicsWorld(b2World *world);

            bool isInSpace() const;

        protected:
            // Fields
            b2Body *_physicsBody;
            AnimatedSprite _sprite;
            sf::FloatRect _spriteBounds;
            bool _flipSprite;

            // Methods
    };
} // space