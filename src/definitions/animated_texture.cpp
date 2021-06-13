#include "animated_texture.hpp"

#include <math.h>
#include <iostream>

#include "../engine.hpp"

namespace space
{
    // Animated Texture
    AnimatedTexture::AnimatedTexture(const DefinitionId &id, const std::string &texturePath, sf::Vector2<ushort> spriteSize) :
        BaseDefinition(id),
        _texturePath(texturePath),
        _spriteSize(spriteSize)
    {

    }

    void AnimatedTexture::addSequence(const std::string &name, const AnimationSequence &sequence)
    {
        _sequences.emplace(std::make_pair(name, sequence));
    }

    sf::IntRect AnimatedTexture::frameRect(uint frame) const
    {
        if (frame >= _spriteRects.size())
            return _spriteRects.front();

        return _spriteRects[frame];
    }

    bool AnimatedTexture::tryGetSequence(const std::string &name, AnimationSequence *result) const
    {
        auto find = _sequences.find(name);
        if (find == _sequences.end())
        {
            *result = AnimationSequence(0, 0, 1);
            return false;
        }

        *result = find->second;
        return true;
    }

    void AnimatedTexture::createSpriteRects()
    {
        auto textureSize = _texture->getSize();
        _spriteCounts.x = textureSize.x / _spriteSize.x;
        _spriteCounts.y = textureSize.y / _spriteSize.y;

        if (textureSize.x % _spriteSize.x != 0)
            std::cout << "Warning animated texture is not a multiple of sprite size x" << std::endl;

        if (textureSize.y % _spriteSize.y != 0)
            std::cout << "Warning animated texture is not a multiple of sprite size y" << std::endl;

        for (auto y = 0; y < _spriteCounts.y; y++)
        for (auto x = 0; x < _spriteCounts.x; x++)
        {
            _spriteRects.emplace_back(x * _spriteSize.x, y * _spriteSize.y, _spriteSize.x, _spriteSize.y);
        }
    }

    void AnimatedTexture::onPostLoad(Engine &engine)
    {
        if (engine.resourceManager().texture(_texturePath, &_texture))
        {
            createSpriteRects();
        }
    }

    // Animation Sequence
    AnimationSequence::AnimationSequence() : AnimationSequence(0, 0, 1.0f)
    {

    }

    AnimationSequence::AnimationSequence(uint startIndex, uint endIndex, float frameTiming) :
        frameTiming(frameTiming),
        totalTime((endIndex - startIndex + 1) * frameTiming)
    {
        _frames.reserve(endIndex - startIndex + 1);
        for (auto i = startIndex; i <= endIndex; ++i)
            _frames.push_back(i);
    }

    AnimationSequence::AnimationSequence(const std::vector<uint> &frames, float frameTiming) :
        frameTiming(frameTiming),
        totalTime(frames.size() * frameTiming),
        _frames(frames)
    {

    }

    uint AnimationSequence::getFrame(float animationTime) const
    {
        if (animationTime < 0)
            return _frames.front();

        auto index = std::floor((animationTime / totalTime) * _frames.size());
        if (index >= _frames.size() || index < 0)
            return _frames.back();

        return _frames[index];
    }

    bool AnimationSequence::areFramesSequential() const
    {
        if (_frames.size() < 2u)
        {
            return false;
        }

        auto f = _frames[0];
        for (auto i = 1u; i < _frames.size(); ++i)
        {
            auto f2 = _frames[i];
            if (f2 - f != 1)
            {
                return false;
            }
            f = f2;
        }

        return true;
    }
} // namespace space
