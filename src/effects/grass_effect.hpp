#pragma once

#include <SFML/Graphics.hpp>
#include "../game/space_object.hpp"

#include "../definitions/grass_effect_definition.hpp"

namespace space
{
    class GameSession;
    class RenderCamera;

    class GrassEffect : public SpaceObject
    {
        public:
            // Fields
            static const SpaceObjectType TypeValue;

            const GrassEffectDefinition &definition;

            // Constructor
            GrassEffect(const ObjectId &id, const GrassEffectDefinition &definition);

            // Methods
            virtual SpaceObjectType type() const { return TypeValue; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneGrassEffect(newId, context); }
            GrassEffect *cloneGrassEffect(const ObjectId &newId, const CloneContext &context);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual bool isGenerated() const { return true; }

        private:
            // Fields
            sf::Sprite _sprite;

            // Methods
    };
} // space