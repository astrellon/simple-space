#include "editor_game_session.hpp"

#include "editor/editor_camera_target.hpp"
#include <valgrind/callgrind.h>

#include "game/character.hpp"
#include "game/space_object.hpp"

#include "keyboard.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "debug/draw_debug.hpp"

namespace space
{
    EditorGameSession::EditorGameSession(Engine &engine) : GameSession(engine)
    {
        _cameraTarget = createObject<EditorCameraTarget>("__EDITOR_CAMERA_TARGET");
    }

    void EditorGameSession::update(sf::Time dt)
    {
    CALLGRIND_TOGGLE_COLLECT;
        for (auto &spaceObject : _spaceObjectsUpdateEveryFrame)
            spaceObject->update(*this, dt, sf::Transform::Identity);

        handleMouse(_cameraTarget);

        const float speed = 150.0f;
        auto seconds = dt.asSeconds();
        auto cameraMove = speed * seconds;
        sf::Vector2f moveTarget;

        if (Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            moveTarget.x -= cameraMove;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            moveTarget.x += cameraMove;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            moveTarget.y -= cameraMove;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            moveTarget.y += cameraMove;
        }

        if (moveTarget != sf::Vector2f())
        {
            _cameraTarget->transform().position += moveTarget;
        }
    CALLGRIND_TOGGLE_COLLECT;
    }

    void EditorGameSession::draw()
    {
    CALLGRIND_TOGGLE_COLLECT;
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
    CALLGRIND_TOGGLE_COLLECT;
    }

    void EditorGameSession::onPostLoad(LoadingContext &context)
    {
        GameSession::onPostLoad(context);

        auto character = _playerController.controllingCharacter();
        if (character != nullptr)
        {
            auto target = Utils::getPosition(character->worldTransform());
            moveSpaceObject(_cameraTarget, target, character->insideArea());
        }
    }

    void EditorGameSession::moveCameraTo(Area &area, sf::Vector2f position)
    {
        moveSpaceObject(_cameraTarget, position, &area);
    }
} // space