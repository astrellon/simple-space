#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"

namespace space
{
    Character::Character(const ObjectId &id, const CharacterDefinition &definition) :
        SpaceObject(id), definition(definition), _rotationSpeed(0)
    {
    }

    void Character::update(Engine &engine, sf::Time dt)
    {
        auto seconds = dt.asSeconds();
        auto movement = Utils::transformDirection(moveInput, _transform.getTransform()) * seconds * definition.speed;
        _rotationSpeed += rotateInput * seconds * 30.0f;

        _transform.position += movement;
        _transform.rotation += _rotationSpeed * seconds;
    }

    void Character::draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        const auto combinedTransform = parentTransform * _transform.getTransform();
        auto elapsedTime = _timeSinceStart.getElapsedTime().asSeconds() * 8;
        auto mod = std::fmod(elapsedTime, static_cast<double>(definition.tiles.length()));
        auto index = static_cast<uint>(mod);

        auto sprite = definition.tiles.sprite(index);
        target.draw(*sprite, combinedTransform);
    }
} // namespace space
