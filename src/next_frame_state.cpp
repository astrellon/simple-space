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
        _generalActions.clear();
    }

    void NextFrameState::addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area)
    {
        _moveSpaceObjects.emplace_back(obj, position, area);
    }

    void NextFrameState::addRemoveObject(const ObjectId &id)
    {
        _removeObjects.push_back(id);
    }

    void NextFrameState::addAction(GeneralAction action)
    {
        _generalActions.push_back(action);
    }

    void NextFrameState::triggerActions()
    {
        for (auto &action : _generalActions)
        {
            action();
        }
    }
} // space