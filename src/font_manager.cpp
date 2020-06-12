#include "font_manager.hpp"

#include <iostream>
#include <filesystem>

#include "utils.hpp"

namespace space
{
    bool FontManager::load(const std::string &name, const std::string &filename)
    {
        if (name.empty())
        {
            return false;
        }

        auto font = std::make_unique<sf::Font>();
        if (!font->loadFromFile(filename))
        {
            return false;
        }

        font.swap(_fonts[name]);
        return true;
    }

    bool FontManager::loadFolder(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();
            auto name = Utils::filenameWithoutExt(filename);

            std::cout << "Loading font: " << name << " from " << filename << std::endl;

            load(name, filename);
        }

        return true;
    }

    const FontManager::FontMap &FontManager::fonts() const
    {
        return _fonts;
    }

    const sf::Font *FontManager::font(const std::string &name) const
    {
        auto find = _fonts.find(name);
        if (find == _fonts.end())
        {
            return nullptr;
        }

        return find->second.get();
    }

}

