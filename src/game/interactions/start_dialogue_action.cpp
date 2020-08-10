#include "start_dialogue_action.hpp"

#include "../../game_session.hpp"

namespace space
{
    StartDialogueAction::StartDialogueAction(const Dialogue *dialogue) : _dialogue(dialogue)
    {

    }

    void StartDialogueAction::execute(GameSession &session)
    {
        session.dialogueManager().startDialogue(_dialogue);
    }
} // namespace space
