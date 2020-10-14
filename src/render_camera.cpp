#include "render_camera.hpp"

#include "engine.hpp"

namespace space
{
    RenderCamera::RenderCamera(Engine &engine, std::string debugName) : _camera(engine, debugName), _created(false)
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
} // namespace space
