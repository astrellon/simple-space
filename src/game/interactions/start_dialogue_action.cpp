#include "start_dialogue_action.hpp"

#include "../../game_session.hpp"

namespace space
{
    StartDialogueAction::StartDialogueAction(const std::string &personTalking, const Dialogue *dialogue) : _dialogue(dialogue), _personTalking(personTalking)
    {

    }

    void StartDialogueAction::execute(GameSession &session)
    {
        session.dialogueManager().startDialogue(_personTalking, _dialogue);
    }
} // namespace space
