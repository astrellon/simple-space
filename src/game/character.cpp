#include "character.hpp"

#include <SFML/System.hpp>
#include <math.h>

#include "../utils.hpp"
#include "../engine.hpp"
#include "../space_transform.hpp"

namespace space
{
    Character::Character(const ObjectId &id, const CharacterDefinition &definition) :
        SpaceObject(id), definition(definition), _rotationSpeed(0), rotateInput(0)
    {
    }

    void Character::update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform)
    {
        auto seconds = dt.asSeconds();

        auto movement = moveInput * seconds * definition.speed;
        _rotationSpeed += rotateInput * seconds * 30.0f;

        _transform.position += movement;
        _transform.rotation += _rotationSpeed * seconds;

        updateWorldTransform(parentTransform);
    }

    void Character::draw(Engine &engine, sf::RenderTarget &target)
    {
        auto elapsedTime = _timeSinceStart.getElapsedTime().asSeconds() * 8;
        auto mod = std::fmod(elapsedTime, static_cast<double>(definition.tiles.length()));
        auto index = static_cast<uint>(mod);

        auto sprite = definition.tiles.sprite(index);
        target.draw(*sprite, _worldTransform);
    }
} // namespace space
