#include "dialogue_manager.hpp"

#include "definitions/dialogue.hpp"

namespace space
{
    DialogueManager::DialogueManager() :
        _current(nullptr),
        _dialogueAudio(nullptr),
        _inDialogue(false),
        _nextPage(0),
        _currentLine("")
    {

    }

    void DialogueManager::startDialogue(const std::string &personTalking, const Dialogue *dialogue, const DialogueAudio *dialogueAudio)
    {
        _personTalking = personTalking;
        _inDialogue = dialogue != nullptr;
        _current = dialogue;
        _nextPage = 0;
        _dialogueAudio = dialogueAudio;
        nextLine();
    }

    bool DialogueManager::tryGetNextLine(std::string *result)
    {
        auto returnResult = nextLine();
        *result = _currentLine;
        return returnResult;
    }

    bool DialogueManager::nextLine()
    {
        if (!_inDialogue || _current == nullptr || static_cast<std::size_t>(_nextPage) >= _current->text.size())
        {
            _inDialogue = false;
            _currentLine = "";

            onNextDialogue.emit();
            return false;
        }

        _currentLine = _current->text[_nextPage++];

        onNextDialogue.emit();
        return true;
    }
} // namespace space
