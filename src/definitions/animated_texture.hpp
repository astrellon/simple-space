#pragma once

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "base_definition.hpp"

namespace space
{
    class AnimationSequence;
    class Engine;

    class AnimatedTexture : public BaseDefinition
    {
        public:
            typedef std::map<std::string, AnimationSequence> SequenceMap;
            // Fields

            // Constructor
            AnimatedTexture(const DefinitionId &id, const std::string &texturePath, sf::Vector2<ushort> spriteSize);

            // Methods
            void addSequence(const std::string &name, const AnimationSequence &sequence);
            const SequenceMap &sequences() const { return _sequences; }

            const sf::Texture *texture() const { return _texture; }
            const std::string &texturePath() const { return _texturePath; }
            sf::IntRect frameRect(uint frame) const;
            sf::Vector2<ushort> spriteSize() const { return _spriteSize; }

            bool tryGetSequence(const std::string &name, AnimationSequence *result) const;
            bool hasSequence(const std::string &name) const;

            static const std::string DefinitionType() { return "animated-texture"; }
            std::string type() const { return DefinitionType(); }
            virtual void onPostLoad(Engine &engine);

        private:
            // Fields
            std::string _texturePath;
            const sf::Texture *_texture;
            std::vector<sf::IntRect> _spriteRects;
            sf::Vector2<ushort> _spriteCounts;
            sf::Vector2<ushort> _spriteSize;
            SequenceMap _sequences;

            // Methods
            void createSpriteRects();
    };

    class AnimationSequence
    {
        public:
            // Fields
            float frameTiming;
            float totalTime;

            // Constructor
            AnimationSequence();
            AnimationSequence(uint startIndex, uint endIndex, float frameTiming);
            AnimationSequence(const std::vector<uint> &frames, float frameTiming);

            // Methods
            uint getFrame(float animationTime) const;

            const std::vector<uint> &frames() const { return _frames; }

            bool areFramesSequential() const;

        private:
            // Fields
            std::vector<uint> _frames;

            // Methods
    };
} // space