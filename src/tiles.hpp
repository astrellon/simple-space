#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "non_copyable.hpp"

namespace space
{
    class Tiles : private NonCopyable
    {
        public:
            void init(const sf::Texture *texture, uint spriteSize, float spriteScale);

            sf::Sprite *getSprite(uint index);
            sf::Vector2f getSpritePosition(uint index) const;
            sf::Vector2f getSpritePosition(uint x, uint y) const;
            const sf::Texture *texture() const;

            typedef std::vector<std::unique_ptr<sf::Sprite>> SpriteList;

        private:
            const sf::Texture *_texture;
            SpriteList _sprites;
            uint _spriteSize;
            float _spriteScale;
            uint _rows;
            uint _columns;

    };
} // town