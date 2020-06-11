#include "texture_manager.hpp"

#include <iostream>
#include <filesystem>

namespace space
{
    const sf::Texture *TextureManager::get(const std::string &filename)
    {
        auto find = _textures.find(filename);
        if (find != _textures.end())
        {
            std::cout << "Texture already loaded: " << filename << std::endl;
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
        texture.swap(_textures[filename]);
        return result;
    }

    void TextureManager::load_folder(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();

            std::cout << "Loading texture: " << filename << std::endl;

            get(filename);
        }
    }

    const TextureManager::TextureMap &TextureManager::textures() const
    {
        return _textures;
    }
}

