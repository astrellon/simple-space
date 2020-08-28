#pragma once

#include "../non_copyable.hpp"

#include <SFML/Graphics.hpp>

namespace space
{

class PostEffect : private NonCopyable
{
    public:
        PostEffect();
        virtual ~PostEffect();
        virtual void apply(const sf::RenderTexture &input, sf::RenderTarget &output) = 0;

        static bool isSupported();

    protected:
        void applyShader(const sf::Shader &shader, sf::RenderTarget &output);

        sf::VertexArray _vertices;
};

} // namespace space