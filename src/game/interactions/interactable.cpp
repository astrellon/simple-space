#include "interactable.hpp"

namespace space
{
    Interactable::Interactable(SpaceObject *parentObject) :
        _parentObject(parentObject), _playerInRange(false)
    {

    }

    void Interactable::onPlayerEnters(GameSession &session)
    {
        _playerInRange = true;
        if (_onPlayerEnters)
        {
            _onPlayerEnters(session);
        }
    }

    void Interactable::onPlayerLeaves(GameSession &session)
    {
        _playerInRange = false;
        if (_onPlayerLeaves)
        {
            _onPlayerLeaves(session);
        }
    }
} // namespace space
