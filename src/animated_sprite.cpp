#include "animated_sprite.hpp"

#include <math.h>
#include <iostream>

namespace space
{
    AnimatedSprite::AnimatedSprite(const AnimatedTexture &texture) :
        animationSpeed(1.0f),
        animationTime(0.0f),
        _texture(texture),
        _sprite(*texture.texture())
    {
        auto size = texture.spriteSize();
        _sprite.setOrigin(sf::Vector2f(size.x * 0.5f, size.y * 0.5f));
    }

    void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_sprite, states);
    }

    void AnimatedSprite::update(sf::Time dt)
    {
        if (dt != sf::Time::Zero)
        {
            animationTime += dt.asSeconds();
            if (animationTime > _currentSequence.totalTime)
            {
                animationTime -= _currentSequence.totalTime;
                if (_nextAnimation.size() > 0)
                {
                    gotoNextSequence(_nextAnimation);
                }
                animationTime = std::fmod(animationTime, _currentSequence.totalTime);
            }
        }

        auto frame = _currentSequence.getFrame(animationTime);
        auto rect = _texture.frameRect(frame);
        _sprite.setTextureRect(rect);
    }

    void AnimatedSprite::sequence(const std::string &name, bool immediateChange)
    {
        if (name == _currentAnimation)
        {
            return;
        }

        if (!immediateChange)
        {
            _nextAnimation = name;
        }
        else
        {
            gotoNextSequence(name);
            animationTime = 0.0f;
        }
    }

    bool AnimatedSprite::hasSequence(const std::string &name) const
    {
        return _texture.hasSequence(name);
    }

    void AnimatedSprite::gotoNextSequence(const std::string &name)
    {
        _currentAnimation = name;
        if (!_texture.tryGetSequence(name, &_currentSequence))
        {
            std::cout << "Unable to find animation sequence " << name << std::endl;
        }
        _nextAnimation = "";
    }

    sf::FloatRect AnimatedSprite::getLocalBounds() const
    {
        float width = static_cast<float>(_texture.spriteSize().x);
        float height = static_cast<float>(_texture.spriteSize().y);

        return sf::FloatRect(0.f, 0.f, width, height);
    }

    sf::FloatRect AnimatedSprite::getGlobalBounds() const
    {
        return _sprite.getTransform().transformRect(getLocalBounds());
    }
} // namespace space
