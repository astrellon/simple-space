#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

#include "non_copyable.hpp"

namespace space
{
    class ResourceManager : private NonCopyable
    {
        public:
            typedef std::map<std::string, std::unique_ptr<sf::Texture>> TextureMap;
            typedef std::map<std::string, std::unique_ptr<sf::Image>> ImageMap;
            typedef std::map<std::string, std::unique_ptr<sf::Font>> FontMap;
            typedef std::map<std::string, std::unique_ptr<sf::Shader>> ShaderMap;

            // Methods
            void loadEmbedded();

            // Fonts
            const sf::Font *defaultFont() const { return _defaultFont.get(); }
            const FontMap &fonts() const { return _fonts; }

            bool font(const std::string &filename, const sf::Font **result);
            const sf::Font *preloadFont(const std::string &filename);
            void preloadFonts(const std::string &folder);

            // Shaders
            const ShaderMap &shaders() const { return _shaders; }

            sf::Shader *preloadShader(const std::string &name, const std::string &vertShaderFilename, const std::string &fragShaderFilename);
            bool shader(const std::string &name, sf::Shader **result);

            // Textures
            const TextureMap &textures() const { return _textures; }

            bool texture(const std::string &filename, const sf::Texture **result);
            bool texture(const std::string &filename, sf::Texture **result);
            sf::Texture *preloadTexture(const std::string &filename);
            void preloadTextures(const std::string &folder);

            // Images
            const ImageMap &images() const { return _images; }

            bool image(const std::string &filename, const sf::Image **result);
            const sf::Image *preloadImage(const std::string &filename);

        private:

            std::unique_ptr<sf::Font> _defaultFont;
            TextureMap _textures;
            ImageMap _images;
            FontMap _fonts;
            ShaderMap _shaders;
    };
}

