#include "planet.hpp"

#include <iostream>

#include "engine.hpp"

namespace space
{
    Planet::Planet(Engine &engine, sf::Texture *texture) : SpaceObject("planet"), _engine(engine), _texture(texture), _shader(nullptr)
    {
        _renderTexture = std::make_unique<sf::RenderTexture>();
        _renderTexture->create(128, 128);
        _renderTexture->setSmooth(false);

        _texture->setRepeated(true);
        _texture->setSmooth(true);
    }

    void Planet::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (_shader == nullptr)
        {
            if (!_engine.resourceManager().shader("planet", &_shader))
            {
                std::cout << "Unable to find shader for planet" << std::endl;
            }
        }
        sf::Sprite sprite(*_texture);

        sf::RenderStates renderState;
        renderState.shader = _shader;

        _shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        _renderTexture->draw(sprite, renderState);
        _renderTexture->display();

        sf::Sprite sphereSprite(_renderTexture->getTexture());
        sphereSprite.setOrigin(64, 64);

        const auto combinedTransform = parentTransform * _transform.getTransform();
        target.draw(sphereSprite, combinedTransform);
    }

} // namespace space
