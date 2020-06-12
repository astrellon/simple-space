#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

#include "non_copyable.hpp"

namespace space
{
    class FontManager : private NonCopyable
    {
        public:
            typedef std::map<std::string, std::unique_ptr<sf::Font>> FontMap;

            bool load(const std::string &name, const std::string &filename);
            bool loadFolder(const std::string &folder);

            const FontMap &fonts() const;
            const sf::Font *font(const std::string &name) const;

        private:

            FontMap _fonts;
    };
}

