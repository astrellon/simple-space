#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "non_copyable.hpp"
#include "definitions/animated_texture.hpp"

namespace space
{
    class AnimatedSprite : public sf::Drawable, private NonCopyable
    {
        public:
            // Fields
            float animationSpeed;
            float animationTime;

            // Constructor
            AnimatedSprite(const AnimatedTexture &texture);

            // Methods
            void update(sf::Time dt);

            void sequence(const std::string &name, bool immediateChange);
            const std::string &currentAnimation() const { return _currentAnimation; }

            inline const AnimatedTexture &texture() const { return _texture; }
            inline sf::IntRect currentRect() const { return _sprite.getTextureRect(); }

        protected:
            // Methods
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            // Fields
            const AnimatedTexture &_texture;
            sf::Sprite _sprite;
            std::string _currentAnimation;
            std::string _nextAnimation;
            AnimationSequence _currentSequence;

            // Methods
            void gotoNextSequence(const std::string &name);
    };
} // space