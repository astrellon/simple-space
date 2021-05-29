#pragma once

#include <string>
#include <vector>
#include <SFML/Audio.hpp>

#include "base_definition.hpp"

namespace space
{
    class Engine;

    class AudioReference
    {
        public:
            // Fields

            // Constructor
            AudioReference(const std::string &filepath, const sf::SoundBuffer *soundBuffer);

            // Methods
            const std::string &audioPath() const { return _audioPath; }
            const sf::SoundBuffer *soundBuffer() const { return _soundBuffer; }
            void onPostLoad(Engine &engine);

        private:
            // Fields
            std::string _audioPath;
            const sf::SoundBuffer *_soundBuffer;


            // Methods
    };

    class DialogueAudio : public BaseDefinition
    {
        public:
            typedef std::vector<AudioReference> SoundList;

            // Fields

            // Constructor
            DialogueAudio(const DefinitionId &id, const SoundList &sounds);

            // Methods
            const SoundList &sounds() const { return _sounds; }
            const sf::SoundBuffer *randomSoundBuffer() const;

            static const std::string DefinitionType() { return "dialogue-audio"; }
            std::string type() const { return DefinitionType(); }
            virtual void onPostLoad(Engine &engine);

        private:
            // Fields
            SoundList _sounds;

            // Methods
    };
} // space