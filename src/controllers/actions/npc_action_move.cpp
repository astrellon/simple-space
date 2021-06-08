#include "npc_action_move.hpp"

#include "../npc_controller.hpp"
#include "../../game/character.hpp"

namespace space
{
    NpcActionMove::NpcActionMove(NpcController *controller, sf::Vector2f destination) :
        NpcAction(controller), _destination(destination), _toDestination(-1.0f)
    {

    }

    void NpcActionMove::update(sf::Time dt)
    {
        auto character = controller()->controllingCharacter();
        if (!character)
        {
            _toDestination = -1.0f;
            character->moveInput = sf::Vector2f();
        }
        else
        {
            auto characterPosition = character->transform().position;
            auto direction = characterPosition - _destination;
            _toDestination = direction.length();

            character->moveInput = direction / -_toDestination;
        }
    }

    bool NpcActionMove::isComplete() const
    {
        return _toDestination >= 0 && _toDestination < 10.0f;
    }

    void NpcActionMove::onComplete()
    {
        auto character = controller()->controllingCharacter();
        if (character)
        {
            character->moveInput = sf::Vector2f();
        }
    }
} // space