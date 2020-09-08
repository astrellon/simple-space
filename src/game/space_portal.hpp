#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "space_object.hpp"
#include "../definitions/space_portal_definition.hpp"
#include "../animated_sprite.hpp"
#include "../effects/portal_shadow.hpp"
#include "../line.hpp"

namespace space
{
    class GameSession;
    class Ship;

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

            void drawPortal(GameSession &session, sf::RenderTarget &target, bool asPolygon);
            void drawPortalOutlines(GameSession &session, sf::RenderTarget &target);

        private:
            // Helpers
            struct NearPortalObject
            {
                Ship *ship;
                sf::Vector2f entryP1;
                sf::Vector2f entryP2;
                int framesOutsideOfRadius;

                NearPortalObject(Ship *ship) : ship(ship), framesOutsideOfRadius(0) { }

                void calcEntryPoints(sf::Vector2f point, float radius);
            };

            // Fields
            AnimatedSprite _sprite;
            std::vector<NearPortalObject> _nearbyObjects;
            PortalShadow _shadow;
            // Is a vector for earcutting
            std::vector<Polygon> _shadowShape;
            std::array<FloatLine, 2> _shadowOutlines;
            std::array<sf::Vector2f, 2> _lerpFromShadowPoint;
            float _lerpFromShadowT;

            // Methods
            void cleanupNearbyObjects();

            NearPortalObject &getNearbyObject(const sf::Vector2f &portalToShip, Ship *ship);
            bool tryGetNearbyObject(Ship *ship, NearPortalObject **result);
    };
} // space