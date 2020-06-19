#include "planet.hpp"

#include <iostream>

#include "engine.hpp"

namespace space
{
    Planet::Planet(const ObjectId &id, const PlanetDefinition &definition)
        : SpaceObject(id), definition(definition), _shader(nullptr)
    {
        _renderTexture = std::make_unique<sf::RenderTexture>();
        _renderTexture->create(definition.size, definition.size);
        _renderTexture->setSmooth(false);

    }

    void Planet::draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (_shader == nullptr)
        {
            if (!engine.resourceManager().shader("planet", &_shader))
            {
                std::cout << "Unable to find shader for planet" << std::endl;
            }
        }
        sf::Sprite sprite(*definition.texture);

        sf::RenderStates renderState;
        renderState.shader = _shader;

        _shader->setUniform("timeSinceStart", engine.timeSinceStart().asSeconds());
        _shader->setUniform("offset", sf::Vector2f(0, 0));
        _shader->setUniform("rotationRate", definition.rotationRate);

        _renderTexture->draw(sprite, renderState);
        _renderTexture->display();

        sf::Sprite sphereSprite(_renderTexture->getTexture());
        sphereSprite.setOrigin(definition.size * 3.0 / 4.0, definition.size / 4.0);

        const auto combinedTransform = parentTransform * _transform.getTransform();
        target.draw(sphereSprite, combinedTransform);
    }

} // namespace space
