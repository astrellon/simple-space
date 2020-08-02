#pragma once

#include "character.hpp"

namespace space
{
    class TeleportClone : public Character
    {
        public:
            // Fields

            // Constructor
            TeleportClone(const ObjectId &id, const Character &clone, const SpaceTransform &preTeleport);

            // Methods
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

        private:
            // Fields
            const ObjectId &_cloneId;
            const SpaceTransform _preTeleport;
            const SpaceTransform _postTeleport;
            Character *_cloneFound;

            // Methods
            bool tryGetClone(GameSession &session, Character **result);
    };
} // space