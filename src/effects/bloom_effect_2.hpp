#pragma once

#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "post_effect.hpp"

#include "../resource_manager.hpp"

namespace space
{
    class BloomEffect2 : public PostEffect
    {
        public:
            BloomEffect2();

            void init(ResourceManager &resourceManager);

            virtual void apply(const sf::RenderTexture &input, sf::RenderTarget &output);

        private:
            typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

        private:
            void prepareTextures(sf::Vector2u size);

            void filterBright(const sf::RenderTexture &input, sf::RenderTarget &output);
            void blurMultipass(RenderTextureArray &renderTextures);
            void blur(const sf::RenderTexture &input, sf::RenderTexture &output, sf::Vector2f offsetFactor);
            void downsample(const sf::RenderTexture &input, sf::RenderTexture &output);
            void add(const sf::RenderTexture &source, const sf::RenderTexture &bloom, sf::RenderTarget &target);

        private:
            //ShaderHolder mShaders;
            sf::Shader *_brightness;
            sf::Shader *_downSample;
            sf::Shader *_blur;
            sf::Shader *_add;

            sf::RenderTexture mBrightnessTexture;
            sf::RenderTexture mDownsampled;
            // RenderTextureArray mFirstPassTextures;
            RenderTextureArray mBlurred;
    };

} // namespace space