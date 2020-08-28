#include "post_effect.hpp"

#include "../debug/draw_debug.hpp"

namespace space
{
    PostEffect::PostEffect() : _vertices(sf::Triangles, 6)
    {
        _vertices[0] = sf::Vertex(sf::Vector2f(-1, 1), sf::Vector2f(0, 1));
        _vertices[1] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[2] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));

        _vertices[3] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[4] = sf::Vertex(sf::Vector2f(1, -1), sf::Vector2f(1, 0));
        _vertices[5] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));
    }

    PostEffect::~PostEffect()
    {
    }

    void PostEffect::applyShader(const sf::Shader &shader, sf::RenderTarget &output)
    {
        auto outputSize = static_cast<sf::Vector2f>(output.getSize());

        sf::RenderStates states;
        states.shader = &shader;
        states.blendMode = sf::BlendNone;

        output.draw(_vertices, states);
        DrawDebug::glDraw++;
    }

    bool PostEffect::isSupported()
    {
        return sf::Shader::isAvailable();
    }
} // namespace space
