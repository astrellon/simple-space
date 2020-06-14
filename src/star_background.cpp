#include "star_background.hpp"

#include "utils.hpp"
#include "engine.hpp"

namespace space
{
    StarBackground::StarBackground(Engine &engine, int numParticles) : Particles(engine, numParticles), _shader(nullptr)
    {

    }


    void StarBackground::onInit()
    {
        for (auto &position : _positions)
        {
            position.x = Utils::randf(-1000, 1000);
            position.y = Utils::randf(-1000, 1000);
        }

        for (auto &colour : _colours)
        {
            colour.r = Utils::randi(127, 235);
            colour.g = Utils::randi(127, 235);
            colour.b = Utils::randi(127, 235);
        }

        if (!_engine.shaderManager().tryGet("particles", &_shader))
        {
            std::cout << "Unable to find shader for star background" << std::endl;
            return;
        }
    }

    void StarBackground::onUpdate(sf::Time dt)
    {

    }

    bool StarBackground::onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (_shader == nullptr)
        {
            return false;
        }

        auto combinedTransform = _engine.camera().view().getTransform() * parentTransform;

        sf::Shader::bind(_shader);

        sf::Glsl::Mat4 mat4(combinedTransform.getMatrix());
        _shader->setUniform("transform", mat4);
        _shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        return true;
    }

    void StarBackground::onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        sf::Shader::bind(nullptr);
    }
} // namespace space
