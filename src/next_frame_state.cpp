#include "next_frame_state.hpp"

namespace space
{
        NextFrameState::NextFrameState()
        {

        }

        void NextFrameState::clear()
        {
            _moveSpaceObjects.clear();
            _removeObjects.clear();
        }

        void NextFrameState::addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area)
        {
            _moveSpaceObjects.emplace_back(obj, position, area);
        }

        void NextFrameState::addRemoveObject(const ObjectId &id)
        {
            _removeObjects.push_back(id);
        }
} // space