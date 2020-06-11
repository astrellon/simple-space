#include "texture_manager.hpp"

#include <iostream>

namespace space
{
    const sf::Texture *TextureManager::load_texture(const std::string &filename)
    {
        auto find = _textures.find(filename);
        if (find != _textures.end())
        {
            return find->second.get();
        }

        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename))
        {
            std::cout << "Unable to find texture: " << filename << std::endl;
            return nullptr;
        }

        texture->setSmooth(false);

        const auto result = texture.get();
        _textures.emplace(filename, std::move(texture));
        return result;
    }

    const TextureManager::TextureMap &TextureManager::textures() const
    {
        return _textures;
    }
}

