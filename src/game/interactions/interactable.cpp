#include "interactable.hpp"

#include "../../controllers/character_controller.hpp"

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

    void Interactable::onCharacterEnters(CharacterController *controller, GameSession &session)
    {
        if (controller->isPlayer())
        {
            _playerInRange = true;
        }

        if (_onCharacterEnters)
        {
            _onCharacterEnters(controller, session);
        }
    }

    void Interactable::onCharacterLeaves(CharacterController *controller, GameSession &session)
    {
        if (controller->isPlayer())
        {
            _playerInRange = false;
        }

        if (_onCharacterLeaves)
        {
            _onCharacterLeaves(controller, session);
        }
    }
} // namespace space
