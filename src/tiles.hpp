#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "non_copyable.hpp"

namespace space
{
    class Tiles : private NonCopyable
    {
        public:
            void init(const sf::Texture *texture, uint spriteSize);

            inline uint length() const { return _length; }
            inline uint rows() const { return _rows; }
            inline uint columns() const { return _columns; }

            sf::Sprite *sprite(uint index);
            const sf::Sprite *sprite(uint index) const;
            sf::Vector2f spritePosition(uint index) const;
            sf::Vector2f spritePosition(uint x, uint y) const;
            const sf::Texture *texture() const;

            typedef std::vector<std::unique_ptr<sf::Sprite>> SpriteList;

        private:
            const sf::Texture *_texture;
            SpriteList _sprites;
            uint _spriteSize;
            uint _rows;
            uint _columns;
            uint _length;

    };
} // town