#include "camera.hpp"

#include "engine.hpp"
#include "game_session.hpp"

namespace space
{
    Camera::Camera(Engine &engine) : _engine(engine), _scale(1.0f), _following(false), _followingRotation(false)
    {

    }

    void Camera::update(sf::Time dt)
    {
        if (_following)
        {
            SpaceObject *followingObject;
            if (_engine.currentSession()->tryGetSpaceObject(_followingId, &followingObject))
            {
                _view.setCenter(followingObject->transform().position);
            }
        }

        auto resetRotation = true;
        if (_followingRotation)
        {
            SpaceObject *followingObject;
            if (_engine.currentSession()->tryGetSpaceObject(_followingRotationId, &followingObject))
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
        _scale = scale;
        _view.setSize(_size / _scale);
    }

    void Camera::size(sf::Vector2f size)
    {
        _size = size;
        _view.setSize(size / _scale);
    }

    void Camera::center(sf::Vector2f center)
    {
        _view.setCenter(center);
    }

    void Camera::followingId(const ObjectId &id)
    {
        _followingId = id;
        _following = true;
    }

    void Camera::following(bool following)
    {
        _following = following;
    }

    void Camera::followingRotationId(const ObjectId &id)
    {
        _followingRotationId = id;
        _followingRotation = true;
    }

    void Camera::followingRotation(bool following)
    {
        _followingRotation = following;
    }

    const sf::View &Camera::view() const
    {
        return _view;
    }
}