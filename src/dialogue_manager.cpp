#include "dialogue_manager.hpp"

#include "definitions/dialogue.hpp"

namespace space
{
    DialogueManager::DialogueManager() : _current(nullptr), _inDialogue(false), _nextPage(0), _currentLine("")
    {

    }

    void DialogueManager::startDialogue(const Dialogue *dialogue)
    {
        _inDialogue = dialogue != nullptr;
        _current = dialogue;
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
        if (!_inDialogue || _current == nullptr || _nextPage >= _current->text.size())
        {
            _inDialogue = false;
            _currentLine = "";
            return false;
        }

        _currentLine = _current->text[_nextPage++];
        return true;
    }
} // namespace space
