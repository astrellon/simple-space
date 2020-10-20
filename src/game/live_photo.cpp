#include "live_photo.hpp"

#include <sstream>

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../utils.hpp"
#include "../render_camera.hpp"

namespace space
{
    LivePhoto::LivePhoto(const ObjectId &id) : SpaceObject(id), _rootObject(nullptr)
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

        // auto angle = (session.engine().timeSinceStart().asSeconds());
        // sf::Vector2f pos(std::cos(angle) * 100, std::sin(angle) * 100);
        // _camera->camera().center(pos);

        if (_rootObject)
        {
            _rootObject->update(session, dt, parentTransform);
        }
    }

    void LivePhoto::draw(GameSession &session, RenderCamera &target)
    {
        if (_rootObject)
        {
            _camera->preDraw();
            _rootObject->draw(session, *_camera.get());

            target.texture().draw(_cameraSprite, _worldTransform);
        }
    }

} // space