#include "dialogue_audio.hpp"

#include "../utils.hpp"

#include "../engine.hpp"
#include "../resource_manager.hpp"

namespace space
{
    AudioReference::AudioReference(const std::string &filepath, const sf::SoundBuffer *soundBuffer):
        _audioPath(filepath), _soundBuffer(soundBuffer)
    {

    }

    void AudioReference::onPostLoad(Engine &engine)
    {
        _soundBuffer = engine.resourceManager().soundBuffer(_audioPath);
    }

    DialogueAudio::DialogueAudio(const DefinitionId &id, const DialogueAudio::SoundList &sounds) : BaseDefinition(id), _sounds(sounds)
    {

    }

    const sf::SoundBuffer *DialogueAudio::randomSoundBuffer() const
    {
        if (_sounds.size() == 0)
        {
            return nullptr;
        }

        auto randomIndex = Utils::randi(0, _sounds.size());
        return _sounds[randomIndex].soundBuffer();
    }

    void DialogueAudio::onPostLoad(Engine &engine)
    {
        for (auto &sound : _sounds)
        {
            sound.onPostLoad(engine);
        }
    }
} // space