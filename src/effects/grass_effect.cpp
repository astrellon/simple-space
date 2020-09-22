#include "grass_effect.hpp"

#include <iostream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../game/character.hpp"
#include "../definition_manager.hpp"
#include "../definitions/shader_definition.hpp"
#include "../definitions/grass_effect_definition.hpp"
#include "../utils.hpp"

namespace space
{
    GrassEffect::GrassEffect(const ObjectId &id, const GrassEffectDefinition &definition) : SpaceObject(id), definition(definition), _sprite(*definition.texture)
    {

    }

    void GrassEffect::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }

    void GrassEffect::draw(GameSession &session, sf::RenderTarget &target)
    {
        if (!_sprite.getTexture())
        {
            return;
        }

        auto &shader = definition.shader->shader;

        sf::RenderStates states;
        states.shader = &shader;
        states.transform = _worldTransform;

        auto size = _sprite.getTexture()->getSize();
        shader.setUniform("tipColour", sf::Glsl::Vec4(definition.tipColour));
        shader.setUniform("sideColour", sf::Glsl::Vec4(definition.sideColour));
        shader.setUniform("windColour", sf::Glsl::Vec4(definition.windColour));
        shader.setUniform("insideScale", Utils::getInsideScale());
        shader.setUniform("worldPosition", sf::Glsl::Vec2(_transform.position));
        shader.setUniform("invTextureSize", sf::Glsl::Vec2(1.0f / size.x, 1.0f / size.y));
        shader.setUniform("timeSinceStart", session.engine().timeSinceStart().asSeconds());
        shader.setUniform("windSpeed", 1.5f);

        auto playerPos = session.playerController().controllingCharacter()->transform().position;
        std::array<sf::Glsl::Vec2, 1> objectPositions;
        objectPositions[0] = sf::Glsl::Vec2(playerPos);
        shader.setUniformArray("objectPositions", objectPositions.data(), 1);

        sf::Vector2f windDirection(-1, -1);
        windDirection = windDirection.normalised();
        shader.setUniform("windDirection", sf::Glsl::Vec2(windDirection));

        target.draw(_sprite, states);
    }
} // space