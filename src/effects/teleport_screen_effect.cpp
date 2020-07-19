#include "teleport_screen_effect.hpp"

#include <iostream>

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

    void TeleportScreenEffect::draw(const sf::Texture *texture, sf::RenderTarget &target)
    {
        if (_dissolve == nullptr)
        {
            return;
        }

        sf::RenderStates states;
        states.shader = _dissolve;
        states.texture = texture;

        _dissolve->setUniform("amount", 0.5f);

        target.draw(_vertices, states);
    }
} // namespace space
