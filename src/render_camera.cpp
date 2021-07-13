#include "render_camera.hpp"

#include "engine.hpp"
#include "debug/draw_debug.hpp"

namespace space
{
    RenderCamera::RenderCamera(Engine &engine, std::string debugName) : _camera(engine, debugName), _created(false),
        _batchBuffer(sf::PrimitiveType::Triangles, 0xFFFF), _batchTexture(nullptr), _batchBufferIndex(0)
    {
    }

    RenderCamera::~RenderCamera()
    {
        std::cout << "Render camera destroyed\n";
    }

    void RenderCamera::update(sf::Time dt)
    {
        _camera.update(dt);

        _texture.setView(_camera.view());
    }

    void RenderCamera::onResize(sf::Vector2f area, float cameraScale)
    {
        _created = true;
        _camera.size(area);

        area *= 1.0f / cameraScale;

        sf::ContextSettings settings;
        settings.stencilBits = 8;

        _texture.create(area.x, area.y, settings);
    }

    void RenderCamera::preDraw()
    {
        //_camera.zoomScale(_camera.en.cameraScale());
        _camera.setZoomScaleFromEngine();
        _texture.setView(_camera.view());
    }

    void RenderCamera::drawSprite(const sf::Sprite &sprite, const sf::Transform &worldTransform)
    {
        auto &vertices = sprite.getVertices();
        if (_batchTexture != sprite.getTexture() || _batchBufferIndex + vertices.size() >= _batchBuffer.getVertexCount())
        {
            commitBatch();
        }

        _batchTexture = sprite.getTexture();

        // static std::vector<sf::Vertex> tempTransform;

        // if (tempTransform.size() < vertices.size())
        // {
        //     tempTransform.reserve(vertices.size());
        // }

        // tempTransform.assign(vertices.data(), vertices.data() + vertices.size());

        for (auto i = 0u; i < vertices.size(); i++)
        {
            _batchBuffer[_batchBufferIndex + i].position = worldTransform * vertices[i].position;
            _batchBuffer[_batchBufferIndex + i].color = vertices[i].color;
            _batchBuffer[_batchBufferIndex + i].texCoords = vertices[i].texCoords;
        }

        // _batchBuffer.update(tempTransform.data(), vertices.size(), _batchBufferIndex);
        _batchBufferIndex += vertices.size();
    }

    void RenderCamera::commitBatch()
    {
        if (_batchBufferIndex > 0 && _batchTexture != nullptr)
        {
            sf::RenderStates states;
            states.texture = _batchTexture;
            // texture().draw(_batchBuffer, 0, _batchBufferIndex, states);
            _batchBuffer.maxDrawCount = _batchBufferIndex;
            texture().draw(_batchBuffer, states);
            _batchBufferIndex = 0;
            _batchTexture = nullptr;
            DrawDebug::glDraw++;
        }
    }
} // namespace space
