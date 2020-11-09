#include "live_photo.hpp"

#include <sstream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"
#include "../serialisers/loading_context.hpp"

namespace space
{
    LivePhotoTarget::LivePhotoTarget(const ObjectId &id) : SpaceObject(id)
    {

    }

    LivePhotoTarget *LivePhotoTarget::cloneLivePhotoTarget(const ObjectId &newId, const CloneContext &context)
    {
        return context.session.createObject<LivePhotoTarget>(newId);
    }

    LivePhoto::LivePhoto(const ObjectId &id) : SpaceObject(id), _targetObject(nullptr)
    {

    }

    LivePhoto *LivePhoto::cloneLivePhoto(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<LivePhoto>(newId);
        populateCloneFromThis(result, context);
        return result;
    }

    void LivePhoto::init(Engine &engine, sf::Vector2f size, float cameraScale)
    {
        std::stringstream cameraId;
        cameraId << "LivePhoto:" << id;
        _camera = std::make_unique<RenderCamera>(engine, cameraId.str());

        _camera->onResize(size * cameraScale, cameraScale);
        _camera->preDraw();
        _camera->camera().center(sf::Vector2f());

        _cameraSprite.setTexture(_camera->texture().getTexture());
        _cameraSprite.setScale(sf::Vector2f(1, -1));
    }

    sf::RenderTexture &LivePhoto::texture() const
    {
        return _camera->texture();
    }

    void LivePhoto::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }

    void LivePhoto::draw(GameSession &session, RenderCamera &target)
    {
        if (_targetObject)
        {
            drawToInternalTexture(session);
            target.texture().draw(_cameraSprite, _worldTransform);
        }
    }

    void LivePhoto::onPostLoad(GameSession &session, LoadingContext &context)
    {
        SpaceObject::onPostLoad(session, context);

        ObjectId liveTargetId;
        if (context.tryGetPostLoadObjectId(id, LoadingType::LivePhotoTarget, liveTargetId))
        {
            SpaceObject *target;
            if (session.tryGetSpaceObject(id, &target))
            {
                targetObject(target);
            }
            else
            {
                std::cout << "Failed to find target object for live photo in session: " << liveTargetId << std::endl;
            }
        }
    }

    void LivePhoto::updateInternalTarget(GameSession &session, sf::Time dt)
    {
        if (_targetObject)
        {
            auto root = _targetObject->rootObject();
            if (root)
            {
                root->update(session, dt, sf::Transform::Identity);
            }
        }
    }

    void LivePhoto::drawToInternalTexture(GameSession &session)
    {
        if (_targetObject)
        {
            _camera->preDraw();
            session.drawAtObject(*_targetObject, _targetObject->transform().position, *_camera.get());
        }
    }



} // space