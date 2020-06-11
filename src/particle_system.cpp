#include "particle_system.hpp"

#include <iostream>

#include "engine.hpp"
#include "utils.hpp"

namespace space
{
    ParticleSystem::ParticleSystem() :
        _numAwake(0), _maxNumberOfParticles(0)
    {

    }

    std::size_t ParticleSystem::numAwake() const
    {
        return _numAwake;
    }

    std::size_t ParticleSystem::maxNumberOfParticles() const
    {
        return _maxNumberOfParticles;
    }
    void ParticleSystem::maxNumberOfParticles(std::size_t number)
    {
        _maxNumberOfParticles = number;
    }

    sf::Sprite *ParticleSystem::sprite()
    {
        return &_sprite;
    }

    void ParticleSystem::update(sf::Time dt)
    {
        auto maxTime = sf::seconds(3.0f);

        for (auto i = 0; i < _numAwake; i++)
        {
            _ages[i] += dt;
            if (_ages[i] > maxTime)
            {
                restart(i);
            }
        }

        auto seconds = dt.asSeconds();
        for (auto i = 0; i < _numAwake; i++)
        {
            _positions[i] += _velocity[i] * seconds;
        }
    }

    void ParticleSystem::draw(Engine *engine, sf::RenderTarget &target)
    {
        auto scale = engine->spriteScale();
        _sprite.setScale(scale, scale);

        for (auto i = 0; i < _numAwake; i++)
        {
            _sprite.setPosition(_positions[i] * scale);
            target.draw(_sprite);
        }
    }

    void ParticleSystem::swap(std::size_t index1, std::size_t index2)
    {
        std::swap(_positions[index1], _positions[index2]);
        std::swap(_velocity[index1], _velocity[index2]);
        std::swap(_ages[index1], _ages[index2]);
    }

    void ParticleSystem::wakeup(std::size_t number)
    {
        auto prevNumAwake = _numAwake;
        _numAwake += number;
        if (_numAwake > _maxNumberOfParticles)
        {
            _maxNumberOfParticles = _numAwake;

            _positions.resize(_numAwake);
            _velocity.resize(_numAwake);
            _ages.resize(_numAwake);

            for (auto index = prevNumAwake; index < _numAwake; ++index)
            {
                restart(index);
            }
        }
    }

    void ParticleSystem::kill(std::size_t index)
    {
        if (index >= _numAwake)
        {
            std::cout << "Out of bounds" << std::endl;
            return;
        }

        if (_numAwake - 1 == index)
        {
            _numAwake--;
        }
        else
        {
            swap(index, _numAwake);
            _numAwake--;
        }
    }

    void ParticleSystem::restart(std::size_t index)
    {
        _positions[index] = sf::Vector2f(50, 50);

        auto dx = Utils::randf(-10.0f, 10.0f);
        auto dy = Utils::randf(-10.0f, 10.0f);
        _velocity[index] = sf::Vector2f(dx, dy);
        _ages[index] = sf::microseconds(0);
    }
} // namespace town
