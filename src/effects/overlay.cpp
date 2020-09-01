#include "overlay.hpp"

#include "../debug/draw_debug.hpp"
#include "../definition_manager.hpp"
#include "../definitions/shader_definition.hpp"

namespace space
{
    Overlay::Overlay() : _shader(nullptr), _vertices(sf::Triangles, 6)
    {
        _vertices[0] = sf::Vertex(sf::Vector2f(-1, 1), sf::Vector2f(0, 1));
        _vertices[1] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[2] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));

        _vertices[3] = sf::Vertex(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
        _vertices[4] = sf::Vertex(sf::Vector2f(1, -1), sf::Vector2f(1, 0));
        _vertices[5] = sf::Vertex(sf::Vector2f(-1, -1), sf::Vector2f(0, 0));
    }

    bool Overlay::init(DefinitionManager &definitionManager)
    {
        ShaderDefinition *overlay;
        if (!definitionManager.tryGet("EFFECT_OVERLAY", &overlay))
        {
            std::cout << "Overlay shader not found!" << std::endl;
            return false;
        }

        _shader = &overlay->shader;
        return true;
    }

    void Overlay::draw(sf::RenderTarget &target, float alpha)
    {
        if (!_shader)
        {
            return;
        }

        _shader->setUniform("alpha", alpha);

        sf::RenderStates states;
        states.shader = _shader;

        target.draw(_vertices, states);
        DrawDebug::glDraw++;
    }
} // namespace space
