#include "npc_controller.hpp"

#include "../game/character.hpp"
#include "../game/interactions/start_dialogue_action.hpp"

#include "./actions/npc_action.hpp"

namespace space
{
    NpcController::NpcController(GameSession &session) : CharacterController(session), _dialogue(nullptr), _startDialogueAction(nullptr)
    {

    }

    void NpcController::update(sf::Time dt)
    {
        _needs.update(dt);

        if (_highLevelActions.size() > 0)
        {
            while (_highLevelActions.size() > 0 && _highLevelActions.front()->isComplete())
            {
                _highLevelActions.front()->onComplete();
                _highLevelActions.pop();
            }
        }

        if (_highLevelActions.size() > 0)
        {
            auto &action = _highLevelActions.front();
            action->update(dt);
        }
    }

    void NpcController::dialogue(const Dialogue *dialogue)
    {
        _dialogue = dialogue;
        if (!_character)
        {
            std::cout << "Unable to attach dialogue without a character" << std::endl;
            return;
        }

        auto interactable = _character->interactable();
        if (interactable)
        {
            if (_startDialogueAction)
            {
                interactable->removeInteraction(_startDialogueAction);
            }

            _startDialogueAction = interactable->createInteraction<StartDialogueAction>(_character->definition.name, dialogue, _character->definition.dialogueAudio);
        }
    }
} // namespace space
