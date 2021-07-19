#include "live_photo.hpp"

#include <sstream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"
#include "../serialisers/loading_context.hpp"
#include "../definitions/compendium_definition.hpp"

namespace space
{
    const SpaceObjectType LivePhotoTarget::TypeValue = SpaceObjectType::LivePhotoTarget;

    LivePhotoTarget::LivePhotoTarget(const ObjectId &id) : SpaceObject(id)
    {

    }

    LivePhotoTarget *LivePhotoTarget::cloneLivePhotoTarget(const ObjectId &newId, const CloneContext &context)
    {
        return context.session.createObject<LivePhotoTarget>(newId);
    }

    SpaceObjectType LivePhoto::TypeValue = SpaceObjectType::LivePhoto;

    LivePhoto::LivePhoto(const ObjectId &id, const sf::Vector2u renderSize) : SpaceObject(id), photoSize(renderSize), _targetObject(nullptr), _lastFrameUpdate(-1), _lastFrameDraw(-1)
    {

    }

    LivePhoto *LivePhoto::cloneLivePhoto(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<LivePhoto>(newId, photoSize);
        populateCloneFromThis(result, context);
        return result;
    }

    void LivePhoto::init(Engine &engine)
    {
        std::stringstream cameraId;
        cameraId << "LivePhoto:" << id;
        _camera = std::make_unique<RenderCamera>(engine, cameraId.str());

        auto area = static_cast<sf::Vector2f>(photoSize) * engine.cameraScale();
        _camera->onResize(area, engine.cameraScale());
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
            if (session.tryGetSpaceObject(liveTargetId, target))
            {
                targetObject(target);

                // For now assume to be in players album
                session.playerController().photoAlbum().addPhoto(this);
                session.playerController().compendium().processNewPhoto(this);
            }
            else
            {
                std::cout << "Failed to find target object for live photo in session: " << liveTargetId << std::endl;
            }
        }
    }

    void LivePhoto::updateInternalTarget(GameSession &session, sf::Time dt)
    {
        auto newFrameCounter = session.engine().frameCounter();
        if (newFrameCounter <= _lastFrameUpdate)
        {
            return;
        }

        _lastFrameUpdate = newFrameCounter;

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
        auto newFrameCounter = session.engine().frameCounter();
        if (newFrameCounter <= _lastFrameDraw)
        {
            return;
        }

        if (_targetObject)
        {
            _lastFrameDraw = newFrameCounter;
            _camera->preDraw();
            session.drawAtObject(*_targetObject, _targetObject->transform().position, *_camera.get());
        }
    }

    std::vector<SpaceObject *> LivePhoto::getObjectsWithCompendium() const
    {
        std::vector<SpaceObject *> result;

        std::cout << "Looping over live photo:" << std::endl;

        _targetObject->rootObject()->loopOver([&](SpaceObject *obj)
        {
            std::cout << "- " << obj->id;
            if (obj->compendiumDefinition())
            {
                std::cout << " comp: " << obj->compendiumDefinition()->id << std::endl;
                result.push_back(obj);
            }
            else
            {
                std::cout << " no comp" << std::endl;
            }

            return true;
        });

        return result;
    }

    void LivePhoto::targetObject(SpaceObject *target)
    {
        _targetObject = target;
        target->rootObject()->loopOver([=](SpaceObject *obj)
        {
            obj->partOfLivePhoto(this);
            return true;
        });
    }

} // space