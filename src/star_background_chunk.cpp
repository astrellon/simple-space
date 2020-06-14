#include "star_background_chunk.hpp"

#include <math.h>

#include "utils.hpp"
#include "engine.hpp"
#include "star_background.hpp"

namespace space
{
    StarBackgroundChunk::StarBackgroundChunk(Engine &engine, const StarBackground &parent) : Particles(engine, parent._numParticlesPerChunk, GL_DYNAMIC_DRAW), _parent(parent)
    {

    }

    void StarBackgroundChunk::reinit()
    {
        auto area = _parent._chunkSize;
        auto distanceScale = _parent._distanceScale;
        sf::Vector2f offset(_position.x * area, _position.y * area);
        auto rand = Utils::randWithSeed((_position.x + _position.y << 16) / distanceScale + distanceScale * 255);

        std::uniform_real_distribution<float> xRange(offset.x, area + offset.x);
        std::uniform_real_distribution<float> yRange(offset.y, area + offset.y);
        std::uniform_real_distribution<float> colourRange(127 * distanceScale * distanceScale, 235 * distanceScale * distanceScale);

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
    }

    void StarBackgroundChunk::onUpdate(sf::Time dt)
    {
        _lastUseTime = _engine.timeSinceStartOnUpdate();
    }

    bool StarBackgroundChunk::onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        if (_parent._shader == nullptr || !isActive())
        {
            return false;
        }

        auto combinedTransform = _engine.camera().view().getTransform() * parentTransform;

        sf::Shader::bind(_parent._shader);

        sf::Glsl::Mat4 mat4(combinedTransform.getMatrix());
        mat4.array[12] *= _parent._distanceScale;
        mat4.array[13] *= _parent._distanceScale;

        _parent._shader->setUniform("transform", mat4);
        _parent._shader->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        return true;
    }

    void StarBackgroundChunk::onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        sf::Shader::bind(nullptr);
    }
} // namespace space
