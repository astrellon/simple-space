#include "layer_camera.hpp"

#include "camera.hpp"

namespace space
{
    LayerCamera::LayerCamera(float distanceScale) :
        _distanceScale(distanceScale), _scale(1.0f)
    {

    }

    void LayerCamera::update(sf::Time dt)
    {
    }

    void LayerCamera::preDraw(const Camera &camera)
    {
        _view.setCenter(camera.center() * _distanceScale);
        if (camera.size() != _size || camera.scale() != _scale)
        {
            size(camera.size(), camera.scale());
        }

        if (camera.isFollowingRotation())
        {
            _view.setRotation(camera.view().getRotation());
        }
        else if (_view.getRotation() != 0.0f)
        {
            _view.setRotation(0.0f);
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
