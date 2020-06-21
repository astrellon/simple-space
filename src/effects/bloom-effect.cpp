#include "bloom-effect.hpp"

namespace space
{
    BloomEffect::BloomEffect()
        : mBrightnessTexture(), mFirstPassTextures(), mSecondPassTextures()
    {
    }

    void BloomEffect::init(ResourceManager &resourceManager)
    {
        _brightness = resourceManager.preloadShader("effect/brightness", "data/shaders/effects/fullpass.vert", "data/shaders/effects/brightness.frag");
        _downSample = resourceManager.preloadShader("effect/downSample", "data/shaders/effects/fullpass.vert", "data/shaders/effects/down-sample.frag");
        _blur = resourceManager.preloadShader("effect/guassianBlur", "data/shaders/effects/fullpass.vert", "data/shaders/effects/guassian-blur.frag");
        _add = resourceManager.preloadShader("effect/add", "data/shaders/effects/fullpass.vert", "data/shaders/effects/add.frag");
    }

    void BloomEffect::apply(const sf::RenderTexture &input, sf::RenderTarget &output)
    {
        prepareTextures(input.getSize());

        filterBright(input, mBrightnessTexture);

        downsample(mBrightnessTexture, mFirstPassTextures[0]);
        blurMultipass(mFirstPassTextures);

        downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
        blurMultipass(mSecondPassTextures);

        add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
        mFirstPassTextures[1].display();
        add(input, mFirstPassTextures[1], output);
    }

    void BloomEffect::prepareTextures(sf::Vector2u size)
    {
        if (mBrightnessTexture.getSize() != size)
        {
            mBrightnessTexture.create(size.x, size.y);
            mBrightnessTexture.setSmooth(true);

            mFirstPassTextures[0].create(size.x / 2, size.y / 2);
            mFirstPassTextures[0].setSmooth(true);
            mFirstPassTextures[1].create(size.x / 2, size.y / 2);
            mFirstPassTextures[1].setSmooth(true);

            mSecondPassTextures[0].create(size.x / 4, size.y / 4);
            mSecondPassTextures[0].setSmooth(true);
            mSecondPassTextures[1].create(size.x / 4, size.y / 4);
            mSecondPassTextures[1].setSmooth(true);
        }
    }

    void BloomEffect::filterBright(const sf::RenderTexture &input, sf::RenderTarget &output)
    {
        _brightness->setUniform("source", input.getTexture());
        applyShader(*_brightness, output);
        //output.display();
    }

    void BloomEffect::blurMultipass(RenderTextureArray &renderTextures)
    {
        sf::Vector2u textureSize = renderTextures[0].getSize();

        for (std::size_t count = 0; count < 2; ++count)
        {
            blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 2.f / textureSize.y));
            blur(renderTextures[1], renderTextures[0], sf::Vector2f(2.f / textureSize.x, 0.f));
        }
    }

    void BloomEffect::blur(const sf::RenderTexture &input, sf::RenderTexture &output, sf::Vector2f offsetFactor)
    {
        _blur->setUniform("source", input.getTexture());
        _blur->setUniform("offsetFactor", offsetFactor);
        applyShader(*_blur, output);
        output.display();
    }

    void BloomEffect::downsample(const sf::RenderTexture &input, sf::RenderTexture &output)
    {
        _downSample->setUniform("source", input.getTexture());
        _downSample->setUniform("sourceSize", sf::Vector2f(input.getSize()));
        applyShader(*_downSample, output);
        output.display();
    }

    void BloomEffect::add(const sf::RenderTexture &source, const sf::RenderTexture &bloom, sf::RenderTarget &output)
    {
        _add->setUniform("source", source.getTexture());
        _add->setUniform("bloom", bloom.getTexture());
        applyShader(*_add, output);
    }
} // namespace space
