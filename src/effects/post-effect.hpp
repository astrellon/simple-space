#pragma once

#include "non_copyable.hpp"

namespace sf
{
    class RenderTarget;
    class RenderTexture;
    class Shader;
} // namespace sf

namespace space
{

class PostEffect : private NonCopyable
{
    public:
        virtual ~PostEffect();
        virtual void apply(const sf::RenderTexture &input, sf::RenderTarget &output) = 0;

        static bool isSupported();

    protected:
        static void applyShader(const sf::Shader &shader, sf::RenderTarget &output);
};

} // namespace space