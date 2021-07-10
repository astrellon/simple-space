#include "editor_game_session.hpp"

#include "editor/editor_camera_target.hpp"

#include "keyboard.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "debug/draw_debug.hpp"

namespace space
{
    EditorGameSession::EditorGameSession(Engine &engine) : GameSession(engine)
    {
        _cameraTarget = createObject<EditorCameraTarget>(nextObjectId());
    }

    void EditorGameSession::update(sf::Time dt)
    {
        for (auto &spaceObject : _spaceObjectsUpdateEveryFrame)
            spaceObject->update(*this, dt, sf::Transform::Identity);

        handleMouse();

        const float speed = 150.0f;
        auto seconds = dt.asSeconds();
        auto cameraMove = speed * seconds;

        if (Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            _cameraTarget->transform().position.x -= cameraMove;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            _cameraTarget->transform().position.x += cameraMove;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            _cameraTarget->transform().position.y -= cameraMove;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            _cameraTarget->transform().position.y += cameraMove;
        }
    }

    void EditorGameSession::draw()
    {
        _renderStack.clear();
        auto &sceneRender = _engine.sceneRender();

        if (DrawDebug::focusOnObject)
        {
            auto pos = Utils::getPosition(DrawDebug::focusOnObject->worldTransform());
            drawAtObject(*DrawDebug::focusOnObject, pos, sceneRender);
        }
        else
        {
            auto pos = Utils::getPosition(_cameraTarget->worldTransform());
            drawAtObject(*_cameraTarget, pos, sceneRender);
        }
    }
} // space