#include "grass_effect.hpp"

#include <iostream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../definition_manager.hpp"
#include "../definitions/shader_definition.hpp"

namespace space
{
    GrassEffect::GrassEffect(const ObjectId &id) : SpaceObject(id), _shader(nullptr), _init(false)
    {

    }

    void GrassEffect::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        if (!_init)
        {
            init(session);
        }

        updateWorldTransform(parentTransform);
    }

    void GrassEffect::draw(GameSession &session, sf::RenderTarget &target)
    {
        if (_shader == nullptr)
        {
            return;
        }

        sf::RenderStates states;
        states.shader = _shader;
        states.transform = _worldTransform;

        auto size = _sprite.getTexture()->getSize();
        _shader->setUniform("tipColour", sf::Glsl::Vec4(sf::Color(207, 255, 112)));
        _shader->setUniform("sideColour", sf::Glsl::Vec4(sf::Color(60, 163, 112)));
        _shader->setUniform("windColour", sf::Glsl::Vec4(sf::Color(233, 255, 153)));
        _shader->setUniform("invTextureSize", sf::Glsl::Vec2(1.0f / size.x, 1.0f / size.y));
        _shader->setUniform("timeSinceStart", session.engine().timeSinceStart().asSeconds());
        _shader->setUniform("windSpeed", 1.5f);
        _shader->setUniform("noiseTex", _noiseTex);

        size = _noiseTex->getSize();
        _shader->setUniform("invNoiseTexSize", sf::Glsl::Vec2(1.0f / size.x, 1.0f / size.y));

        sf::Vector2f windDirection(-1, -1);
        windDirection = windDirection.normalised();
        _shader->setUniform("windDirection", sf::Glsl::Vec2(windDirection));

        target.draw(_sprite, states);
    }

    void GrassEffect::init(GameSession &session)
    {
        _init = true;
        ShaderDefinition *shader;
        if (!session.engine().definitionManager().tryGet("EFFECT_GRASS", &shader))
        {
            std::cout << "Unable to find grass shader for grass effect" << std::endl;
        }
        else
        {
            _shader = &shader->shader;
        }

        sf::Texture *noiseTex;
        if (!session.engine().resourceManager().texture("data/textures/noise.png", &noiseTex))
        {
            std::cout << "Unable to find noise texture for grass effect" << std::endl;
        }
        else
        {
            noiseTex->setRepeated(true);
            _noiseTex = noiseTex;
        }
    }
} // space