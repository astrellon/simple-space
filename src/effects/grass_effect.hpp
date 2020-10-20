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
            const GrassEffectDefinition &definition;

            // Constructor
            GrassEffect(const ObjectId &id, const GrassEffectDefinition &definition);

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, GameSession &session) { return cloneGrassEffect(newId, session); }
            GrassEffect *cloneGrassEffect(const ObjectId &newId, GameSession &session);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual bool isGenerated() const { return true; }

            static const std::string SpaceObjectType() { return "grass-effect"; }
            virtual std::string type() const { return SpaceObjectType(); }

        private:
            // Fields
            sf::Sprite _sprite;

            // Methods
    };
} // space