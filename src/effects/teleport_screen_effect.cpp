#include "teleport_screen_effect.hpp"

#include <iostream>

#include "../game_session.hpp"
#include "../engine.hpp"

namespace space
{
    TeleportScreenEffect::TeleportScreenEffect() : _dissolve(nullptr), _vertices(sf::Triangles, 6)
    {
        _vertices[0] = sf::Vertex(sf::Vector2f(-1, 1), sf::Vector2f(0, 1));
        _vertices[1] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[2] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));

        _vertices[3] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[4] = sf::Vertex(sf::Vector2f(1, -1), sf::Vector2f(1, 0));
        _vertices[5] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));
    }

    void TeleportScreenEffect::init(ResourceManager &resourceManager)
    {
        _dissolve = resourceManager.preloadShader("teleport_dissolve", "data/shaders/effects/fullpass.vert", "data/shaders/effects/teleport_dissolve.frag");
        if (!_dissolve)
        {
            std::cout << "Dissolve shader not found!" << std::endl;
        }
    }

    void TeleportScreenEffect::draw(GameSession &session, const sf::Texture *texture, sf::RenderTarget &target, float t)
    {
        if (_dissolve == nullptr)
        {
            return;
        }

        sf::RenderStates states;
        states.shader = _dissolve;
        states.texture = texture;

        _dissolve->setUniform("amount", 1.0f - t);
        _dissolve->setUniform("timeSinceStart", session.engine().timeSinceStart().asSeconds());

        target.draw(_vertices, states);
    }
} // namespace space
