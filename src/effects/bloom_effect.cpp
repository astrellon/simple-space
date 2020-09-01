#include "bloom_effect.hpp"

#include "../definition_manager.hpp"
#include "../definitions/shader_definition.hpp"

namespace space
{
    BloomEffect::BloomEffect()
        : _brightnessTexture(), _firstPassTextures(), _secondPassTextures(), _hasAllShaders(false), _brightness(nullptr), _downSample(nullptr), _blur(nullptr), _add(nullptr)
    {
    }

    bool BloomEffect::init(DefinitionManager &definitionManager)
    {
        _hasAllShaders = false;
        ShaderDefinition *brightness, *downSample, *blur, *add;
        if (!definitionManager.tryGet("EFFECT_BRIGHTNESS", &brightness))
        {
            std::cout << "Missing brightness shader for bloom effect" << std::endl;
            return false;
        }

        if (!definitionManager.tryGet("EFFECT_DOWNSAMPLE", &downSample))
        {
            std::cout << "Missing downsample shader for bloom effect" << std::endl;
            return false;
        }
        if (!definitionManager.tryGet("EFFECT_GUASSIAN_BLUR", &blur))
        {
            std::cout << "Missing guassian blur shader for bloom effect" << std::endl;
            return false;
        }
        if (!definitionManager.tryGet("EFFECT_ADD", &add))
        {
            std::cout << "Missing add shader for bloom effect" << std::endl;
            return false;
        }

        _brightness = &brightness->shader;
        _downSample = &downSample->shader;
        _blur = &blur->shader;
        _add = &add->shader;

        _hasAllShaders = true;
        return true;
    }

    void BloomEffect::apply(const sf::RenderTexture &input, sf::RenderTarget &output)
    {
        if (!_hasAllShaders)
        {
            return;
        }

        prepareTextures(input.getSize());

        filterBright(input, _brightnessTexture);

        downsample(_brightnessTexture, _firstPassTextures[0]);
        blurMultipass(_firstPassTextures);

        downsample(_firstPassTextures[0], _secondPassTextures[0]);
        blurMultipass(_secondPassTextures);

        add(_firstPassTextures[0], _secondPassTextures[0], _firstPassTextures[1]);
        _firstPassTextures[1].display();
        add(input, _firstPassTextures[1], output);
    }

    void BloomEffect::prepareTextures(sf::Vector2u size)
    {
        if (_brightnessTexture.getSize() != size)
        {
            _brightnessTexture.create(size.x, size.y);
            _brightnessTexture.setSmooth(true);

            _firstPassTextures[0].create(size.x / 2, size.y / 2);
            _firstPassTextures[0].setSmooth(true);
            _firstPassTextures[1].create(size.x / 2, size.y / 2);
            _firstPassTextures[1].setSmooth(true);

            _secondPassTextures[0].create(size.x / 4, size.y / 4);
            _secondPassTextures[0].setSmooth(true);
            _secondPassTextures[1].create(size.x / 4, size.y / 4);
            _secondPassTextures[1].setSmooth(true);
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
