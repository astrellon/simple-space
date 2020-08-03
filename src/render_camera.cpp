#include "render_camera.hpp"

#include "engine.hpp"

namespace space
{
    RenderCamera::RenderCamera(Engine &engine, std::string debugName) : _camera(engine, debugName), transitionData(nullptr)
    {

    }

    void RenderCamera::update(sf::Time dt)
    {
        _camera.update(dt);

        _texture.setView(_camera.view());
    }

    void RenderCamera::onResize(sf::Vector2f area, float cameraScale)
    {
        _camera.size(area);

        area *= 1.0f / cameraScale;

        _texture.create(area.x, area.y);
    }

    void RenderCamera::preDraw()
    {
        _texture.setView(_camera.view());
    }
} // namespace space
