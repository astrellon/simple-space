#include "next_frame_state.hpp"

namespace space
{
        NextFrameState::NextFrameState() : nextStarSystem(nullptr), nextPlanetSurface(nullptr) {}

        void NextFrameState::clear()
        {
            nextStarSystem = nullptr;
            nextPlanetSurface = nullptr;

            _moveSpaceObjects.clear();
        }

        void NextFrameState::addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area)
        {
            _moveSpaceObjects.emplace_back(obj, position, area);
        }
} // space