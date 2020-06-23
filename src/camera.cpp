#include "camera.hpp"

#include "engine.hpp"
#include "game_session.hpp"

namespace space
{
    Camera::Camera(Engine &engine) : _engine(engine), _scale(1.0f)
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

    void Camera::following(const ObjectId &id)
    {
        _followingId = id;
        _following = true;
    }

    void Camera::following(bool following)
    {
        _following = following;
    }

    const sf::View &Camera::view() const
    {
        return _view;
    }
}