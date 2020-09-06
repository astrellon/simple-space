#pragma once

#include <SFML/Graphics.hpp>

#include "space_object.hpp"
#include "../definitions/space_portal_definition.hpp"
#include "../animated_sprite.hpp"
#include "../effects/portal_shadow.hpp"

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

            void ignoreId(const ObjectId &id);

        private:
            // Helpers
            struct IgnoreId
            {
                ObjectId id;
                int framesOutsideOfRadius;

                IgnoreId(const ObjectId &id) : id(id), framesOutsideOfRadius(0) { }
            };

            // Fields
            AnimatedSprite _sprite;
            std::vector<IgnoreId> _idsToIgnore;
            std::vector<SpaceObject *> _nearbyObjects;
            PortalShadow _shadow;
            Polygon _shadowShape;

            // Methods
            bool foundId(const ObjectId &id);
    };
} // space