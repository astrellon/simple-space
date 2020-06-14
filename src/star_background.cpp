#include "star_background.hpp"

#include <random>

#include "engine.hpp"
#include "utils.hpp"

namespace space
{
    StarBackground::StarBackground(Engine &engine, int numParticles) : Particles(engine, numParticles, GL_STREAM_DRAW), _shader(nullptr)
    {

    }

    void StarBackground::onInit()
    {
        std::uniform_real_distribution<float> xRange(-2000, 2000);
        std::uniform_real_distribution<float> yRange(-1200, 1200);
        std::uniform_real_distribution<float> colourRange(127, 235);
        auto rand = Utils::randWithSeed(500);

        _prevCameraCenter = _engine.camera().view().getCenter();

        auto distanceFactor = 1 / static_cast<float>(_numParticles);
        for (auto i = 0; i < _numParticles; i++)
        {
            auto &position = _positions[i];
            auto &colour = _colours[i];

            position.x = xRange(rand);
            position.y = yRange(rand);

            auto distance = i * distanceFactor;

            colour.r = static_cast<int>(colourRange(rand)) * distance;
            colour.g = static_cast<int>(colourRange(rand)) * distance;
            colour.b = static_cast<int>(colourRange(rand)) * distance;
            //colour.a = distance;
        }

        if (!_engine.resourceManager().shader("stars", &_shader))
        {
            std::cout << "Unable to find shader for star background" << std::endl;
            return;
        }
    }

    void StarBackground::onUpdate(sf::Time dt)
    {
        auto center = _engine.camera().view().getCenter();
        if (center == _prevCameraCenter)
        {
            return;
        }

        auto delta = _prevCameraCenter - center;

        _prevCameraCenter = center;

        auto distanceFactor = 1 / static_cast<float>(_numParticles);
        for (auto i = 0; i < _numParticles; i++)
        {
            auto &position = _positions[i];

            auto distance = i * distanceFactor;
            position.x += distance * delta.x;
            position.y += distance * delta.y;

            while(position.x < -2000) { position.x += 4000; }
            while(position.x >  2000) { position.x -= 4000; }
            while(position.y < -1200) { position.y += 2400; }
            while(position.y >  1200) { position.y -= 2400; }
        }

        syncPositions();
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
        mat4.array[12] = 0;
        mat4.array[13] = 0;

        _shader->setUniform("transform", mat4);
        _shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        return true;
    }

    void StarBackground::onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        sf::Shader::bind(nullptr);
    }
} // namespace space
