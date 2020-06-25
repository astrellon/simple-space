#include "layer_camera.hpp"

#include "camera.hpp"

namespace space
{
    LayerCamera::LayerCamera(const Camera &camera, float distanceScale) :
        _camera(camera), _distanceScale(distanceScale), _scale(1.0f)
    {

    }

    void LayerCamera::update(sf::Time dt)
    {
        _view.setCenter(_camera.center() * _distanceScale);
        if (_camera.size() != _size || _camera.scale() != _scale)
        {
            size(_camera.size(), _camera.scale());
        }
    }

    void LayerCamera::size(const sf::Vector2f &size, float scale)
    {
        _size = size;
        _scale = scale;

        auto layerScale = scale > 1.0f ? 2.0f : 1.0f;
        _view.setSize(size / layerScale);
    }
} // namespace space
