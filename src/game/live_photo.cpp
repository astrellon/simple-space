#include "live_photo.hpp"

#include <sstream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"

namespace space
{
    LivePhotoTarget::LivePhotoTarget(const ObjectId &id) : SpaceObject(id)
    {

    }

    LivePhotoTarget *LivePhotoTarget::cloneLivePhotoTarget(const ObjectId &newId, GameSession &session)
    {
        return session.createObject<LivePhotoTarget>(newId);
    }

    LivePhoto::LivePhoto(const ObjectId &id) : SpaceObject(id), _targetObject(nullptr)
    {

    }

    LivePhoto *LivePhoto::cloneLivePhoto(const ObjectId &newId, GameSession &session)
    {
        return session.createObject<LivePhoto>(newId);
    }

    void LivePhoto::init(Engine &engine, sf::Vector2f size, float cameraScale)
    {
        std::stringstream cameraId;
        cameraId << "LivePhoto:" << id;
        _camera = std::make_unique<RenderCamera>(engine, cameraId.str());

        _camera->onResize(size, cameraScale);
        _camera->preDraw();
        _camera->camera().center(sf::Vector2f());

        _cameraSprite.setTexture(_camera->texture().getTexture());
    }

    void LivePhoto::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);

        if (_targetObject)
        {
            _targetObject->update(session, dt, parentTransform);
        }
    }

    void LivePhoto::draw(GameSession &session, RenderCamera &target)
    {
        if (_targetObject)
        {
            _camera->preDraw();

            session.drawAtObject(*_targetObject, _targetObject->transform().position, *_camera.get());
            target.texture().draw(_cameraSprite, _worldTransform);
        }
    }

} // space