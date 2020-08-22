#include "npc_controller.hpp"

#include "../game/character.hpp"
#include "../game/interactions/start_dialogue_action.hpp"

namespace space
{
    NpcController::NpcController(GameSession &session) : CharacterController(session), _dialogue(nullptr), _startDialogueAction(nullptr)
    {

    }

    void NpcController::update(sf::Time dt)
    {

    }

    void NpcController::dialogue(const Dialogue *dialogue)
    {
        _dialogue = dialogue;
        if (!_character)
        {
            std::cout << "Unable to attach dialogue without a character" << std::endl;
            return;
        }

        if (_startDialogueAction)
        {
            _character->interactable().removeInteraction(_startDialogueAction);
        }

        _startDialogueAction = _character->interactable().createInteraction<StartDialogueAction>(dialogue);
    }
} // namespace space
