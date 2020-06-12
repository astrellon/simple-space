#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

#include "non_copyable.hpp"

namespace space
{
    class TextureManager : private NonCopyable
    {
        public:
            typedef std::map<std::string, std::unique_ptr<sf::Texture>> TextureMap;

            const sf::Texture *get(const std::string &filename);
            void loadFolder(const std::string &folder);

            const TextureMap &textures() const;

        private:

            TextureMap _textures;
    };
}

