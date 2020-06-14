#include "star_background.hpp"

#include <math.h>

#include "engine.hpp"

namespace space
{
    StarBackground::StarBackground(Engine &engine, float chunkSize, int numParticlesPerChunk, float distanceScale) :
        _engine(engine), _chunkSize(chunkSize), _numParticlesPerChunk(numParticlesPerChunk), _distanceScale(distanceScale)
    {

    }

    void StarBackground::update(sf::Time dt)
    {
        auto center = _engine.camera().view().getCenter() * _distanceScale;
        auto size = _engine.camera().view().getSize() * 0.6f;

        auto lowerX = (int)floor((center.x - size.x) / _chunkSize);
        auto upperX = (int)ceil((center.x + size.x) / _chunkSize);
        auto lowerY = (int)floor((center.y - size.y) / _chunkSize);
        auto upperY = (int)ceil((center.y + size.y) / _chunkSize);

        // std::cout << "X: [" << lowerX << ", " << upperX << "] Y: [" << lowerY << ", " << upperY << "]" << std::endl;

        for (auto x = lowerX; x <= upperX; x++)
        {
            for (auto y = lowerY; y <= upperY; y++)
            {
                sf::Vector2i pos(x, y);
                auto chunk = getChunk(pos);
                if (chunk->position() != pos)
                {
                    chunk->position(pos);
                    chunk->reinit();
                }
                chunk->update(dt);
            }
        }
    }

    void StarBackground::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        for (auto &chunk : _chunkList)
        {
            chunk->draw(target, parentTransform);
        }
    }

    StarBackgroundChunk *StarBackground::getChunk(sf::Vector2i pos)
    {
        for (auto &it : _chunkList)
        {
            if (!it->isActive())
            {
                return it.get();
            }
        }

        auto newChunk = std::make_unique<StarBackgroundChunk>(_engine, _numParticlesPerChunk, _chunkSize, _distanceScale);
        newChunk->position(pos);

        auto result = newChunk.get();
        _chunkList.emplace_back(std::move(newChunk));

        return result;
    }
} // namespace space
