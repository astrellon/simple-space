#pragma once

#include <SFML/Graphics.hpp>

#include "space_object.hpp"
#include "../definitions/space_portal_definition.hpp"
#include "../animated_sprite.hpp"

namespace space
{
    class GameSession;

    class SpacePortal : public SpaceObject
    {
        public:
            // Fields
            const SpacePortalDefinition &definition;
            ObjectId targetObjectId;

            // Constructor
            SpacePortal(const ObjectId &id, const SpacePortalDefinition &definition);

            // Methods
            static const std::string SpaceObjectType() { return SpacePortalDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

        private:
            // Fields
            AnimatedSprite _sprite;

            // Methods
    };
} // space