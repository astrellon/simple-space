#pragma once

#include <SFML/Graphics.hpp>
#include "../game/space_object.hpp"

#include "../definitions/grass_effect_definition.hpp"

namespace space
{
    class GameSession;

    class GrassEffect : public SpaceObject
    {
        public:
            // Fields
            const GrassEffectDefinition &definition;

            // Constructor
            GrassEffect(const ObjectId &id, const GrassEffectDefinition &definition);

            // Methods
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);
            virtual bool isGenerated() const { return true; }

            static const std::string SpaceObjectType() { return "grass-effect"; }
            virtual std::string type() const { return SpaceObjectType(); }

        private:
            // Fields
            sf::Sprite _sprite;

            // Methods
    };
} // space