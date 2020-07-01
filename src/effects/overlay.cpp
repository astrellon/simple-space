#include "overlay.hpp"

#include "../engine.hpp"

namespace space
{
    Overlay::Overlay(Engine &engine) : _shader(nullptr), _vertices(sf::Triangles, 6)
    {
        _shader = engine.resourceManager().preloadShader("effect/overlay", "data/shaders/effects/overlay.vert", "data/shaders/effects/overlay.frag");

        _vertices[0] = sf::Vertex(sf::Vector2f(-1, 1), sf::Vector2f(0, 1));
        _vertices[1] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[2] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));

        _vertices[3] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[4] = sf::Vertex(sf::Vector2f(1, -1), sf::Vector2f(1, 0));
        _vertices[5] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));
    }

    void Overlay::draw(sf::RenderTarget &target, float alpha)
    {
        _shader->setUniform("alpha", alpha);

        sf::RenderStates states;
        states.shader = _shader;

        target.draw(_vertices, states);
    }
} // namespace space
