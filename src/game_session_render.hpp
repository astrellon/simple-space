#pragma once

namespace space
{
    class SpaceObject;

    class GameSessionRender
    {
        public:
            // Fields
            SpaceObject *target;
            SpaceObject *ignoreObject;
            SpaceObject *prevPortalTarget;
            int portalLevel;

            // Constructor
            GameSessionRender(SpaceObject *target) : target(target), ignoreObject(nullptr), portalLevel(0), prevPortalTarget(nullptr) { }

            // Methods
    };
} // space