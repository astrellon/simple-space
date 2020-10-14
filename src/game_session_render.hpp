#pragma once

#include <cassert>

namespace space
{
    class SpaceObject;

    class GameSessionRender
    {
        public:
            // Fields

            // Constructor
            GameSessionRender(SpaceObject &target, RenderCamera &renderTarget, int portalLevel, SpaceObject *prevPortalTarget) :
                _target(target), _renderTarget(renderTarget), _portalLevel(portalLevel), _prevPortalTarget(prevPortalTarget)
            {
                if (portalLevel <= 0)
                {
                    throw std::runtime_error("Oh no!");
                }
            }

            // Methods
            inline SpaceObject &target() const { return _target; }
            inline RenderCamera &renderTarget() const { return _renderTarget; }
            inline int portalLevel() const { return _portalLevel; }
            inline SpaceObject *prevPortalTarget() const { return _prevPortalTarget; }

        private:
            // Fields
            SpaceObject &_target;
            RenderCamera &_renderTarget;
            int _portalLevel;
            SpaceObject *_prevPortalTarget;

            // Methods
    };
} // space