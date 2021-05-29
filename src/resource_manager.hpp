#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <tmxlite/Map.hpp>

#include <string>
#include <map>

#include "non_copyable.hpp"
#include "shader_resource.hpp"

namespace space
{
    class ResourceManager : private NonCopyable
    {
        public:
            typedef std::map<std::string, std::unique_ptr<sf::Texture>> TextureMap;
            typedef std::map<std::string, std::unique_ptr<sf::Image>> ImageMap;
            typedef std::map<std::string, std::unique_ptr<sf::Font>> FontMap;
            typedef std::map<std::string, std::unique_ptr<sf::SoundBuffer>> SoundBufferMap;
            typedef std::map<std::string, std::unique_ptr<std::string>> TextMap;
            typedef std::map<std::string, std::unique_ptr<tmx::Map>> TmxMap;

            // Methods

            // Fonts
            const FontMap &fonts() const { return _fonts; }

            bool font(const std::string &filename, const sf::Font **result);
            const sf::Font *font(const std::string &filename);

            const sf::Font *preloadFont(const std::string &filename);
            void preloadFonts(const std::string &folder);

            // SoundBuffers
            const SoundBufferMap &soundBuffers() const { return _soundBuffers; }

            bool soundBuffer(const std::string &filename, const sf::SoundBuffer **result);
            const sf::SoundBuffer *soundBuffer(const std::string &filename);

            const sf::SoundBuffer *preloadSoundBuffer(const std::string &filename);
            void preloadSoundBuffers(const std::string &folder);

            bool getRandomSound(const std::string &baseFilename, const sf::SoundBuffer **result);

            // Texts
            const TextMap &texts() const { return _texts; }

            bool text(const std::string &filename, const std::string **result);

            // Textures
            const TextureMap &textures() const { return _textures; }

            bool texture(const std::string &filename, const sf::Texture **result);
            bool texture(const std::string &filename, sf::Texture **result);
            const sf::Texture *texture(const std::string &filename);

            sf::Texture *preloadTexture(const std::string &filename);
            void preloadTextures(const std::string &folder);

            // Images
            const ImageMap &images() const { return _images; }

            bool image(const std::string &filename, const sf::Image **result);
            const sf::Image *preloadImage(const std::string &filename);

            // Maps
            const TmxMap &maps() const { return _maps; }

            bool map(const std::string &filename, tmx::Map **result);
            tmx::Map *preloadMap(const std::string &filename);
            void preloadMaps(const std::string &folder);

        private:

            TextureMap _textures;
            ImageMap _images;
            FontMap _fonts;
            SoundBufferMap _soundBuffers;
            TextMap _texts;
            TmxMap _maps;
    };
}

