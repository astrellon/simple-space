#include "camera.hpp"

#include "engine.hpp"
#include "game_session.hpp"

namespace space
{
    Camera::Camera(Engine &engine) : _engine(engine)
    {

    }

    void Camera::update(sf::Time dt)
    {
        if (_following)
        {
            SpaceObject *followingObject;
            if (_engine.currentSession()->tryGetSpaceObject(_followingId, &followingObject))
            {
                const auto &trans = followingObject->transform().getMatrix();
                _view.setCenter(sf::Vector2f(trans[3], trans[7]));
            }
        }
    }

    void Camera::setSize(sf::Vector2f size)
    {
        _view.setSize(size);
    }

    void Camera::setFollowing(const ObjectId &id)
    {
        _followingId = id;
        _following = true;
    }

    void Camera::setFollowing(bool following)
    {
        _following = following;
    }

    const sf::View &Camera::view() const
    {
        return _view;
    }
}