#include "tiles.hpp"

#include "engine.hpp"

namespace space
{
    void Tiles::init(const sf::Texture *texture, uint spriteSize, bool center)
    {
        _texture = texture;

        auto size = texture->getSize();
        _rows = size.y / spriteSize;
        _columns = size.x / spriteSize;
        _length = _rows * _columns;

        _spriteSize = spriteSize;

        auto halfSize = spriteSize / 2.0f;

        for (auto y = 0u; y < _rows; y++)
        {
            for (auto x = 0u; x < _columns; x++)
            {
                auto rect = sf::IntRect(x * spriteSize, y * spriteSize, spriteSize, spriteSize);

                auto sprite = std::make_unique<sf::Sprite>(*texture, rect);
                if (center)
                {
                    sprite->setOrigin(halfSize, halfSize);
                }
                _sprites.push_back(std::move(sprite));
            }
        }
    }

    sf::Sprite *Tiles::sprite(uint index)
    {
        if (index > _sprites.size())
        {
            return nullptr;
        }

        return _sprites[index].get();
    }

    const sf::Sprite *Tiles::sprite(uint index) const
    {
        if (index > _sprites.size())
        {
            return nullptr;
        }

        return _sprites[index].get();
    }

    sf::Vector2f Tiles::spritePosition(uint index) const
    {
        if (_sprites.size() <= index)
        {
            return sf::Vector2f(0, 0);
        }

        auto x = index % _columns;
        auto y = index / _rows;

        return sf::Vector2f(x * _spriteSize, y * _spriteSize);
    }

    sf::Vector2f Tiles::spritePosition(uint x, uint y) const
    {
        return sf::Vector2f(x * _spriteSize, y * _spriteSize);
    }

    const sf::Texture *Tiles::texture() const
    {
        return _texture;
    }
}