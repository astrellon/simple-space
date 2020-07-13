#include "planet.hpp"

#include <algorithm>
#include <iostream>

#include "../engine.hpp"
#include "../game_session.hpp"
#include "planet_surface.hpp"

namespace space
{
    Planet::Planet(const ObjectId &id, const PlanetDefinition &definition)
        : CelestialBody(id, definition), definition(definition), _shader(nullptr)
    {
        _renderTexture = std::make_unique<sf::RenderTexture>();
        _renderTexture->create(definition.size, definition.size);
        _renderTexture->setSmooth(false);
    }

    void Planet::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        if (!_initedSurfaces)
        {
            _initedSurfaces = true;

            for (auto surfaceId : definition.planetSurfaceIds)
            {
                PlanetSurface *surface;
                if (session.tryGetPlanetSurface(surfaceId, &surface))
                {
                    addPlanetSurface(surface);
                }
            }
        }
        updateWorldTransform(parentTransform);
    }

    void Planet::draw(GameSession &session, sf::RenderTarget &target)
    {
        if (_shader == nullptr)
        {
            if (!session.engine().resourceManager().shader("planet", &_shader))
            {
                std::cout << "Unable to find shader for planet" << std::endl;
            }
        }

        setTransformFromLocation();

        sf::Sprite sprite(*definition.texture);
        sprite.setScale(definition.size / 2, definition.size / 2);
        sprite.setTextureRect(sf::IntRect(0, 0, 2, 2));

        sf::RenderStates renderState;
        renderState.shader = _shader;
        renderState.blendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::SrcAlpha);

        _shader->setUniform("timeSinceStart", session.engine().timeSinceStart().asSeconds());
        _shader->setUniform("offset", 0.0f);
        _shader->setUniform("scale", definition.scale);
        _shader->setUniform("oscillateNoise", definition.oscillateNoise);

        sf::Glsl::Vec4 colour(definition.glowColour);
        _shader->setUniform("glowColour", colour);
        _shader->setUniform("rotationRate", definition.rotationRate);

        _renderTexture->clear();
        _renderTexture->draw(sprite, renderState);
        _renderTexture->display();

        sf::Sprite sphereSprite(_renderTexture->getTexture());
        sphereSprite.setOrigin(definition.size / 2.0, definition.size / 2.0);

        sf::RenderStates states;
        states.transform = _worldTransform;

        target.draw(sphereSprite, states);
    }

    void Planet::addPlanetSurface(PlanetSurface *planetSurface)
    {
        _planetSurfaces.push_back(planetSurface);
        planetSurface->partOfPlanet(this);
    }
    void Planet::removePlanetSurface(PlanetSurface *planetSurface)
    {
        auto find = std::find(_planetSurfaces.begin(), _planetSurfaces.end(), planetSurface);
        if (find != _planetSurfaces.end())
        {
            _planetSurfaces.erase(find);
        }

        if (planetSurface->partOfPlanet() == this)
        {
            planetSurface->partOfPlanet(nullptr);
        }
    }
} // namespace space
