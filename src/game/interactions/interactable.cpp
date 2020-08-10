#include "interactable.hpp"

namespace space
{
    Interactable::Interactable(SpaceObject *parentObject) :
        _parentObject(parentObject), _playerInRange(false)
    {

    }

    bool Interactable::removeInteraction(Interaction *interaction)
    {
        for (auto iter = _interactions.begin(); iter != _interactions.end(); ++iter)
        {
            if (iter->get() == interaction)
            {
                _interactions.erase(iter);
                return true;
            }
        }

        return false;
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
