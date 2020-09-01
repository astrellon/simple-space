#include "next_frame_state.hpp"

namespace space
{
        NextFrameState::NextFrameState() : nextStarSystem(nullptr), nextPlanetSurface(nullptr) {}

        void NextFrameState::clear()
        {
            nextStarSystem = nullptr;
            nextPlanetSurface = nullptr;

            _moveCharacters.clear();
            _moveSpaceObjects.clear();
        }

        void NextFrameState::addMoveCharacter(Character *character, sf::Vector2f position, WalkableArea *area)
        {
            _moveCharacters.emplace_back(character, position, area);
        }

        void NextFrameState::addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, StarSystem *starSystem)
        {
            _moveSpaceObjects.emplace_back(obj, position, starSystem);
        }
} // space