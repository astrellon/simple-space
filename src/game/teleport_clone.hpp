#pragma once

#include "character.hpp"

namespace space
{
    class RenderCamera;

    class TeleportClone : public Character
    {
        public:
            // Fields

            // Constructor
            TeleportClone(const ObjectId &id, const Character &clone, const SpaceTransform &preTeleport);

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, GameSession &session) { return cloneTeleportClone(newId, session); }
            TeleportClone *cloneTeleportClone(const ObjectId &newId, GameSession &session);

            static const std::string SpaceObjectType() { return "teleport-clone"; }
            std::string type() const { return SpaceObjectType(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);

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