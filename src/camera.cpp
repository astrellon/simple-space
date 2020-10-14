#include "camera.hpp"

#include "engine.hpp"
#include "game_session.hpp"

namespace space
{
    CameraProps::CameraProps() : scale(1.0f), following(false), followingRotation(false)
    {

    }

    Camera::Camera(Engine &engine, std::string debugName) : debugName(debugName), _engine(engine), _zoomScale(1.0f)
    {

    }

    void Camera::update(sf::Time dt)
    {
        setZoomScaleFromEngine();

        if (_props.following)
        {
            SpaceObject *followingObject;
            if (_engine.currentSession()->tryGetSpaceObject(_props.followingId, &followingObject))
            {
                auto trans = followingObject->worldTransform();
                sf::Vector2f pos(trans.getMatrix()[12], trans.getMatrix()[13]);
                _view.setCenter(pos);
                //std::cout << "Camera [" << debugName << "]: " << pos.x << ", " << pos.y << std::endl;
            }
            else
            {
                std::cout << "Camera [" << debugName << "]: Not found!" << std::endl;
            }
        }

        auto resetRotation = true;
        if (_props.followingRotation)
        {
            SpaceObject *followingObject;
            if (_engine.currentSession()->tryGetSpaceObject(_props.followingRotationId, &followingObject))
            {
                resetRotation = false;
                _view.setRotation(followingObject->transform().rotation);
            }
        }

        if (resetRotation && _view.getRotation() != 0.0f)
        {
            _view.setRotation(0.0f);
        }
    }

    void Camera::scale(float scale)
    {
        if (scale != _props.scale)
        {
            _props.scale = scale;
            updateViewSize();
        }
    }

    void Camera::zoomScale(float scale)
    {
        if (scale != _zoomScale)
        {
            _zoomScale = scale;
            updateViewSize();
        }
    }

    void Camera::setZoomScaleFromEngine()
    {
        zoomScale(_engine.cameraScale());
    }

    void Camera::size(sf::Vector2f size)
    {
        _size = size;
        updateViewSize();
    }

    void Camera::center(sf::Vector2f center)
    {
        _view.setCenter(center);
    }

    void Camera::rotation(float rotation)
    {
        _view.setRotation(rotation);
    }

    void Camera::followingId(const ObjectId &id)
    {
        _props.followingId = id;
        _props.following = true;
    }

    void Camera::following(bool following)
    {
        _props.following = following;
    }

    void Camera::followingRotationId(const ObjectId &id)
    {
        _props.followingRotationId = id;
        _props.followingRotation = true;
    }

    void Camera::followingRotation(bool following)
    {
        _props.followingRotation = following;
    }

    const sf::View &Camera::view() const
    {
        return _view;
    }

    float Camera::getRotation() const
    {
        if (_props.followingRotation)
        {
            SpaceObject *followingObject;
            if (_engine.currentSession()->tryGetSpaceObject(_props.followingRotationId, &followingObject))
            {
                return followingObject->transform().rotation;
            }
        }

        return 0.0f;
    }

    void Camera::cameraProps(const CameraProps &props)
    {
        _props = props;
        updateViewSize();
    }

    void Camera::updateViewSize()
    {
        auto size = _size / (_props.scale * _zoomScale);
        _view.setSize(size);
    }

    sf::FloatRect Camera::viewport() const
    {
        auto size = _view.getSize();
        auto center = _view.getCenter();
        return sf::FloatRect(center.x - size.x * 0.5f, center.y - size.y * 0.5f, size.x, size.y);
    }
}