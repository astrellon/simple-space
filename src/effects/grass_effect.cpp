#include "grass_effect.hpp"

#include <iostream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../game/character.hpp"
#include "../definition_manager.hpp"
#include "../definitions/shader_definition.hpp"
#include "../definitions/grass_effect_definition.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"

namespace space
{
    GrassEffect::GrassEffect(const ObjectId &id, const GrassEffectDefinition &definition) : SpaceObject(id), definition(definition), _sprite(*definition.texture)
    {

    }

    GrassEffect *GrassEffect::cloneGrassEffect(const ObjectId &newId, GameSession &session)
    {
        auto result = session.createObject<GrassEffect>(newId, definition);
        result->transform(_transform);
        return result;
    }

    void GrassEffect::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }

    void GrassEffect::draw(GameSession &session, RenderCamera &target)
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
        shader.setUniform("insideScale", Utils::InsideScale);
        shader.setUniform("worldPosition", sf::Glsl::Vec2(_transform.position));
        shader.setUniform("invTextureSize", sf::Glsl::Vec2(1.0f / size.x, 1.0f / size.y));
        shader.setUniform("timeSinceStart", session.engine().timeSinceStart().asSeconds());
        shader.setUniform("windSpeed", 1.5f);

        auto playerPos = session.playerController().controllingCharacter()->transform().position;
        std::array<sf::Glsl::Vec2, 2> objectPositions;
        objectPositions[0] = sf::Glsl::Vec2(playerPos + sf::Vector2f(-1.5f, 1.0f));
        objectPositions[1] = sf::Glsl::Vec2(playerPos + sf::Vector2f(1.5f, 1.0f));
        shader.setUniformArray("objectPositions", objectPositions.data(), objectPositions.size());

        sf::Vector2f windDirection(-1, -1);
        windDirection = windDirection.normalised();
        shader.setUniform("windDirection", sf::Glsl::Vec2(windDirection));

        target.texture().draw(_sprite, states);
    }
} // space