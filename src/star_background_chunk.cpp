#include "star_background_chunk.hpp"

#include "utils.hpp"
#include "engine.hpp"

namespace space
{
    StarBackgroundChunk::StarBackgroundChunk(Engine &engine, int numParticles, float area, float distanceScale) : Particles(engine, numParticles, GL_DYNAMIC_DRAW), _area(area), _shader(nullptr), _distanceScale(distanceScale)
    {

    }

    void StarBackgroundChunk::reinit()
    {
        sf::Vector2f offset(_position.x * _area, _position.y * _area);
        auto rand = Utils::randWithSeed((_position.x + _position.y << 16) / _distanceScale + _distanceScale * 255);

        std::uniform_real_distribution<float> xRange(offset.x, _area + offset.x);
        std::uniform_real_distribution<float> yRange(offset.y, _area + offset.y);
        std::uniform_real_distribution<float> colourRange(127 * _distanceScale * _distanceScale, 235 * _distanceScale * _distanceScale);

        for (auto &position : _positions)
        {
            position.x = xRange(rand);
            position.y = yRange(rand);
        }

        for (auto &colour : _colours)
        {
            colour.r = static_cast<int>(colourRange(rand));
            colour.g = static_cast<int>(colourRange(rand));
            colour.b = static_cast<int>(colourRange(rand));
        }

        if (_inited)
        {
            syncPositionsColours();
        }
    }

    bool StarBackgroundChunk::isActive() const
    {
        return _lastUseTime >= _engine.timeSinceStartOnUpdate();
    }

    void StarBackgroundChunk::onInit()
    {
        reinit();

        if (!_engine.shaderManager().tryGet("stars", &_shader))
        {
            std::cout << "Unable to find shader for star background" << std::endl;
            return;
        }
    }

    void StarBackgroundChunk::onUpdate(sf::Time dt)
    {
        _lastUseTime = _engine.timeSinceStartOnUpdate();
    }

    bool StarBackgroundChunk::onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (_shader == nullptr || !isActive())
        {
            return false;
        }

        auto combinedTransform = _engine.camera().view().getTransform() * parentTransform;

        sf::Shader::bind(_shader);

        sf::Glsl::Mat4 mat4(combinedTransform.getMatrix());
        mat4.array[12] *= _distanceScale;
        mat4.array[13] *= _distanceScale;

        _shader->setUniform("transform", mat4);
        _shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        return true;
    }

    void StarBackgroundChunk::onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        sf::Shader::bind(nullptr);
    }
} // namespace space
