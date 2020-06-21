#include "bloom-effect-2.hpp"

namespace space
{
    BloomEffect2::BloomEffect2()
        : mBrightnessTexture(), mBlurred(), mDownsampled()
    {
    }

    void BloomEffect2::init(ResourceManager &resourceManager)
    {
        _brightness = resourceManager.preloadShader("effect/brightness", "data/shaders/effects/fullpass.vert", "data/shaders/effects/brightness.frag");
        _downSample = resourceManager.preloadShader("effect/downSample", "data/shaders/effects/fullpass.vert", "data/shaders/effects/down-sample.frag");
        _blur = resourceManager.preloadShader("effect/guassianBlur", "data/shaders/effects/fullpass.vert", "data/shaders/effects/guassian-blur.frag");
        _add = resourceManager.preloadShader("effect/add", "data/shaders/effects/fullpass.vert", "data/shaders/effects/add.frag");
    }

    void BloomEffect2::apply(const sf::RenderTexture &input, sf::RenderTarget &output)
    {
        prepareTextures(input.getSize());

        downsample(input, mDownsampled);
        mDownsampled.display();
        mDownsampled.generateMipmap();

        filterBright(mDownsampled, mBlurred[0]);

        //blur(mBrightnessTexture, mBlurred, );
        blurMultipass(mBlurred);

        // downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
        // blurMultipass(mSecondPassTextures);

        //add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
        //mFirstPassTextures[1].display();
        mBlurred[0].display();
        add(input, mBlurred[0], output);
    }

    void BloomEffect2::prepareTextures(sf::Vector2u size)
    {
        if (mBrightnessTexture.getSize() != size)
        {
            mBrightnessTexture.create(size.x, size.y);
            mBrightnessTexture.setSmooth(true);

            mDownsampled.create(size.x / 4, size.y / 4);
            mDownsampled.setSmooth(true);

            mBlurred[0].create(size.x / 4, size.y / 4);
            mBlurred[0].setSmooth(true);
            mBlurred[1].create(size.x / 4, size.y / 4);
            mBlurred[1].setSmooth(true);
        }
    }

    void BloomEffect2::filterBright(const sf::RenderTexture &input, sf::RenderTarget &output)
    {
        _brightness->setUniform("source", input.getTexture());
        applyShader(*_brightness, output);
    }

    void BloomEffect2::blurMultipass(RenderTextureArray &renderTextures)
    {
        sf::Vector2u textureSize = renderTextures[0].getSize();

        for (std::size_t count = 0; count < 1; ++count)
        {
            blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
            blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
        }
    }

    void BloomEffect2::blur(const sf::RenderTexture &input, sf::RenderTexture &output, sf::Vector2f offsetFactor)
    {
        _blur->setUniform("source", input.getTexture());
        _blur->setUniform("offsetFactor", offsetFactor);
        applyShader(*_blur, output);
        output.display();
    }

    void BloomEffect2::downsample(const sf::RenderTexture &input, sf::RenderTexture &output)
    {
        _downSample->setUniform("source", input.getTexture());
        _downSample->setUniform("sourceSize", sf::Vector2f(input.getSize()));
        applyShader(*_downSample, output);
        output.display();
    }

    void BloomEffect2::add(const sf::RenderTexture &source, const sf::RenderTexture &bloom, sf::RenderTarget &output)
    {
        _add->setUniform("source", source.getTexture());
        _add->setUniform("bloom", bloom.getTexture());
        applyShader(*_add, output);
    }
} // namespace space
