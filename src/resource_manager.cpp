#include "resource_manager.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "utils.hpp"

namespace space
{
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
            *result = nullptr;
            return false;
        }

        *result = loadFont;
        return true;
    }

    const sf::Font *ResourceManager::font(const std::string &filename)
    {
        const sf::Font *result = nullptr;
        font(filename, &result);
        return result;
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

    // SoundBuffers
    bool ResourceManager::soundBuffer(const std::string &filename, const sf::SoundBuffer **result)
    {
        auto find = _soundBuffers.find(filename);
        if (find != _soundBuffers.end())
        {
            *result = find->second.get();
            return true;
        }

        auto loadSoundBuffer = preloadSoundBuffer(filename);
        if (loadSoundBuffer == nullptr)
        {
            *result = nullptr;
            return false;
        }

        *result = loadSoundBuffer;
        return true;
    }

    const sf::SoundBuffer *ResourceManager::soundBuffer(const std::string &filename)
    {
        const sf::SoundBuffer *result = nullptr;
        soundBuffer(filename, &result);
        return result;
    }

    const sf::SoundBuffer *ResourceManager::preloadSoundBuffer(const std::string &filename)
    {
        auto soundBuffer = std::make_unique<sf::SoundBuffer>();
        if (!soundBuffer->loadFromFile(filename))
        {
            return nullptr;
        }

        auto result = soundBuffer.get();
        soundBuffer.swap(_soundBuffers[filename]);
        return result;
    }

    void ResourceManager::preloadSoundBuffers(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();

            std::cout << "Loading sound buffer: " << filename << std::endl;

            preloadSoundBuffer(filename);
        }
    }

    bool ResourceManager::getRandomSound(const std::string &baseFilename, const sf::SoundBuffer **result)
    {
        std::vector<sf::SoundBuffer *> potentialSoundBuffers;

        for (auto &iter : _soundBuffers)
        {
            if (iter.first.find(baseFilename) == 0)
            {
                potentialSoundBuffers.push_back(iter.second.get());
            }
        }

        if (potentialSoundBuffers.size() == 0)
        {
            *result = nullptr;
            return false;
        }

        auto randomIndex = Utils::randi(0, potentialSoundBuffers.size());
        *result = potentialSoundBuffers[randomIndex];
        return true;
    }

    // Texts
    bool ResourceManager::text(const std::string &filename, const std::string **result)
    {
        auto find = _texts.find(filename);
        if (find != _texts.end())
        {
            *result = find->second.get();
            return true;
        }

        std::ifstream in(filename, std::ios::in | std::ios::binary);
        if (in)
        {
            std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

            auto fileSize = ifs.tellg();
            ifs.seekg(0, std::ios::beg);

            std::vector<char> bytes(fileSize);
            ifs.read(bytes.data(), fileSize);

            auto contents = std::make_unique<std::string>(bytes.data(), fileSize);
            *result = contents.get();
            _texts[filename] = std::move(contents);
            return true;
        }

        return false;
    }

    // Textures
    bool ResourceManager::texture(const std::string &filename, const sf::Texture **result)
    {
        sf::Texture *tempResult;
        const bool loaded = texture(filename, &tempResult);
        *result = tempResult;
        return loaded;
    }
    bool ResourceManager::texture(const std::string &filename, sf::Texture **result)
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
    const sf::Texture *ResourceManager::texture(const std::string &filename)
    {
        const sf::Texture *result = nullptr;
        texture(filename, &result);
        return result;
    }

    sf::Texture *ResourceManager::preloadTexture(const std::string &filename)
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
            auto ext = Utils::getFilenameExt(filename);
            if (Utils::iequals(ext, "aseprite"))
            {
                // Can silently ignore aseprite files
                continue;
            }

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

    // Maps
    bool ResourceManager::map(const std::string &filename, tmx::Map **result)
    {
        auto find = _maps.find(filename);
        if (find != _maps.end())
        {
            *result = find->second.get();
            return true;
        }

        *result = preloadMap(filename);
        return *result != nullptr;
    }

    tmx::Map *ResourceManager::preloadMap(const std::string &filename)
    {
        auto map = std::make_unique<tmx::Map>();
        map->load(filename);

        auto result = map.get();
        map.swap(_maps[filename]);
        return result;
    }

    void ResourceManager::preloadMaps(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();
            auto ext = Utils::getFilenameExt(filename);
            if (ext != "tmx")
            {
                continue;
            }

            std::cout << "Loading map: " << filename << std::endl;

            preloadMap(filename);
        }
    }
}

