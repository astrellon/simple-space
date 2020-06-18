#include "resource_manager.hpp"

#include <iostream>
#include <filesystem>

#include "utils.hpp"
#include "embeddedData/font.h"

namespace space
{
    void ResourceManager::loadEmbedded()
    {
        _defaultFont = std::make_unique<sf::Font>();
        if (!_defaultFont->loadFromMemory(LiberationSans_Regular_ttf, LiberationSans_Regular_ttf_len))
        {
            std::cout << "Error loading default font!" << std::endl;
        }
    }

    // Fonts
    bool ResourceManager::font(const std::string &filename, const sf::Font **result)
    {
        auto find = _fonts.find(filename);
        if (find != _fonts.end())
        {
            *result = find->second.get();
            return true;
        }

        auto loadFont = preloadFont(filename);
        if (loadFont == nullptr)
        {
            *result = _defaultFont.get();
            return false;
        }

        *result = loadFont;
        return true;
    }

    const sf::Font *ResourceManager::preloadFont(const std::string &filename)
    {
        auto font = std::make_unique<sf::Font>();
        if (!font->loadFromFile(filename))
        {
            return nullptr;
        }

        auto result = font.get();
        font.swap(_fonts[filename]);
        return result;
    }

    void ResourceManager::preloadFonts(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();

            std::cout << "Loading font: " << filename << std::endl;

            preloadFont(filename);
        }
    }

    // Shaders
    sf::Shader *ResourceManager::preloadShader(const std::string &name, const std::string &vertShaderFilename, const std::string &fragShaderFilename)
    {
        auto find = _shaders.find(name);
        if (find != _shaders.end())
        {
            std::cout << "Shader already loaded: " << name << std::endl;
            return find->second.get();
        }

        auto shader = std::make_unique<sf::Shader>();
        if (!shader->loadFromFile(vertShaderFilename, fragShaderFilename))
        {
            std::cout << "Unable to load shader: " << vertShaderFilename << ", " << fragShaderFilename << std::endl;
            return nullptr;
        }

        auto result = shader.get();
        shader.swap(_shaders[name]);
        return result;
    }

    bool ResourceManager::shader(const std::string &name, sf::Shader **result)
    {
        auto find = _shaders.find(name);
        if (find != _shaders.end())
        {
            *result = find->second.get();
            return true;
        }

        return false;
    }

    // Textures
    bool ResourceManager::texture(const std::string &filename, const sf::Texture **result)
    {
        auto find = _textures.find(filename);
        if (find != _textures.end())
        {
            *result = find->second.get();
            return true;
        }

        *result = preloadTexture(filename);
        return *result != nullptr;
    }

    const sf::Texture *ResourceManager::preloadTexture(const std::string &filename)
    {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename))
        {
            std::cout << "Unable to find texture: " << filename << std::endl;
            return nullptr;
        }

        texture->setSmooth(false);

        auto result = texture.get();
        texture.swap(_textures[filename]);
        return result;
    }

    void ResourceManager::preloadTextures(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();

            std::cout << "Loading texture: " << filename << std::endl;

            preloadTexture(filename);
        }
    }

     // Images
    bool ResourceManager::image(const std::string &filename, const sf::Image **result)
    {
        auto find = _images.find(filename);
        if (find != _images.end())
        {
            *result = find->second.get();
            return true;
        }

        *result = preloadImage(filename);
        return *result != nullptr;
    }

    const sf::Image *ResourceManager::preloadImage(const std::string &filename)
    {
        auto image = std::make_unique<sf::Image>();
        if (!image->loadFromFile(filename))
        {
            std::cout << "Unable to find image: " << filename << std::endl;
            return nullptr;
        }

        auto result = image.get();
        image.swap(_images[filename]);
        return result;
    }
}
