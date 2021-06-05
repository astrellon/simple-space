#include "star_background.hpp"

#include <random>
#include <algorithm>

#include <SFML/OpenGL.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "engine.hpp"
#include "utils.hpp"
#include "render_camera.hpp"
#include "debug/draw_debug.hpp"

namespace space
{
    StarBackground::StarBackground(Engine &engine, const StarBackgroundOptions &options) :
        _engine(engine), _options(options), _backgroundColour(options.backgroundColour)
    {
        _layers.emplace_back(std::make_unique<StarBackgroundLayer>(*this));

        _backgroundColour.init(engine.definitionManager());
    }

    void StarBackground::update(sf::Time dt)
    {
        for (auto &layer : _layers)
        {
            layer->update(dt);
        }
    }

    void StarBackground::draw(RenderCamera &target)
    {
        _backgroundColour.draw(target.texture(), 1.0f);

        glMatrixMode(GL_PROJECTION);

        auto cameraView = target.camera().view();
        auto aspect = cameraView.getSize().x / cameraView.getSize().y;

        glm::mat4 Projection = glm::perspective(glm::pi<float>() * DrawDebug::fov, aspect, DrawDebug::drawClose, DrawDebug::drawFar);
        glLoadMatrixf(&Projection[0][0]);

        auto trans = cameraView.getInverseTransform();
        auto center = Utils::getPosition(trans);
        glRotatef(cameraView.getRotation(), 0, 0, 1.0f);
        glTranslatef(-center.x, center.y, 0);

        glMatrixMode(GL_MODELVIEW);

        shader()->setUniform("timeSinceStart", _engine.timeSinceStart().asSeconds());

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        for (auto &layer : _layers)
        {
            layer->draw(target);
        }

        glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    }

    // StarBackgroundLayer
    StarBackgroundLayer::StarBackgroundLayer(StarBackground &parent) :
          _parent(parent), _camera()
    {

    }

    void StarBackgroundLayer::update(sf::Time dt)
    {
        _camera.update(dt);

        auto chunk = getChunk(sf::Vector2i());
        chunk->active(true);
        // auto center = _camera.view().getCenter();
        // auto size = _camera.view().getSize() * 0.6f;
        // auto lowerX = (int)floor((center.x - size.x) / _parent.area());
        // auto upperX = (int)ceil((center.x + size.x) / _parent.area());
        // auto lowerY = (int)floor((center.y - size.y) / _parent.area());
        // auto upperY = (int)ceil((center.y + size.y) / _parent.area());
        // sf::IntRect bounds(lowerX, lowerY, upperX - lowerX, upperY - lowerY);

        // for (auto &chunk : _chunks)
        // {
        //     auto inBounds = bounds.contains(chunk->position());
        //     chunk->active(inBounds);
        // }

        // for (auto x = lowerX; x <= upperX; x++)
        // {
        //     for (auto y = lowerY; y <= upperY; y++)
        //     {
        //         sf::Vector2i pos(x, y);
        //         auto chunk = getChunk(pos);
        //         if (chunk->position() != pos)
        //         {
        //             chunk->position(pos);
        //             chunk->active(true);
        //         }
        //     }
        // }
    }
    void StarBackgroundLayer::draw(RenderCamera &renderCamera)
    {
        auto shader = _parent.shader();
        sf::RenderStates states;
        states.shader = shader;

        auto pointSize = std::max(1, static_cast<int>(std::roundf(0.4f * renderCamera.camera().scale())));

        shader->setUniform("pointSize", pointSize);

        for (auto &chunk : _chunks)
        {
            if (!chunk->isActive())
            {
                continue;
            }

            chunk->draw(renderCamera, states);
        }
    }

    StarBackgroundChunk *StarBackgroundLayer::getChunk(sf::Vector2i pos)
    {
        for (auto &it : _chunks)
        {
            if (it->position() == pos)
            {
                return it.get();
            }
        }

        for (auto &it : _chunks)
        {
            if (!it->isActive())
            {
                return it.get();
            }
        }

        auto newChunk = std::make_unique<StarBackgroundChunk>(*this);
        newChunk->position(pos);

        auto result = newChunk.get();
        _chunks.emplace_back(std::move(newChunk));

        return result;
    }

    // Chunks
    StarBackgroundChunk::StarBackgroundChunk(StarBackgroundLayer &parent) :
        _parent(parent), _vertexBuffer(sf::Points), _active(false)
    {
    }

    void StarBackgroundChunk::position(sf::Vector2i position)
    {
        _position = position;

        auto area = _parent.area();
        auto numParticles = _parent.numParticles();
        auto seed = (position.x + (position.y << 16)) + 17;

        auto rand = Utils::randWithSeed(seed);

        auto positionOffset = sf::Vector3f(position.x, position.y, 0) * area;

        std::uniform_real_distribution<float> posRange(0, area);
        std::uniform_real_distribution<float> depthRange(0, 100);
        std::uniform_real_distribution<float> colourRange(120, 220);

        auto starColourTexture = _parent.parent().options().starColours;
        auto starColourTextureSize = starColourTexture ? starColourTexture->getSize() : sf::Vector2u(0, 0);
        std::uniform_real_distribution<float> textureColourRange(0, starColourTextureSize.y);

        _verticies.reserve(numParticles);

        for (auto i = 0; i < numParticles; i++)
        {
            auto &vertex = _verticies[i];

            // auto x = posRange(rand);
            // auto y = posRange(rand);
            // auto z = depthRange(rand);
            auto x = i * 2;
            auto y = i;
            auto z = -50;
            vertex.position = sf::Vector3f(x, y, z) + positionOffset;

            sf::Color colour;
            if (starColourTexture)
            {
                auto index = static_cast<int>(textureColourRange(rand));
                colour = starColourTexture->getPixel(0, index);
            }
            else
            {
                colour.r = static_cast<sf::Uint8>(colourRange(rand));
                colour.g = static_cast<sf::Uint8>(colourRange(rand));
                colour.b = static_cast<sf::Uint8>(colourRange(rand));
            }
            vertex.color = colour;
        }

        if (!_vertexBuffer.create(numParticles))
        {
            throw std::runtime_error("Unable to create vertex buffer for star background");
        }
        if (!_vertexBuffer.update(_verticies.data(), numParticles, 0))
        {
            throw std::runtime_error("Error updating vertex buffer for star background");
        }
    }

    void StarBackgroundChunk::draw(RenderCamera &target, sf::RenderStates &states)
    {
        target.texture().draw(_vertexBuffer, states);
        DrawDebug::glDraw++;
    }
} // namespace space
