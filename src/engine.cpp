#include "engine.hpp"

#include <iostream>
#include <fstream>
#include <chrono>

#include "utils.hpp"
#include "game_session.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "particles.hpp"
#include "debug/draw_debug.hpp"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "game/items/placed_item.hpp"
#include "game/items/placeable_item.hpp"
#include "game/ship.hpp"

#include "game-ui/game_ui_manager.hpp"

#include "effects/overlay.hpp"
#include "effects/bloom_effect.hpp"

#include "ui/ui_manager.hpp"
#include "ui/ui_debug.hpp"
#include "ui/ui_photo_album.hpp"

#include "definitions/cursor.hpp"

namespace space
{
    Engine::Engine(sf::RenderWindow *window) :
        enableBloom(true),

        _sceneRender(nullptr),
        _sceneRenderTransition(nullptr),

        _spriteScale(1.0f),
        _cameraScale(2.0f),
        _timeScale(1.0f),
        _initedImgui(false),
        _headlessMode(window == nullptr),
        _currentSession(nullptr),
        _window(window),
        _deltaTime(sf::Time::Zero),
        _timeSinceStartOnUpdate(sf::Time::Zero),
        _frameCounter(0)
    {
        _resourceManager = std::make_unique<ResourceManager>();
        _definitionManager = std::make_unique<DefinitionManager>();
        _uiManager = std::make_unique<UIManager>();
        _gameUIManager = std::make_unique<GameUIManager>(*this);

        _frameStart = std::chrono::high_resolution_clock::now();

        _sceneRender = _renderCameras.create(*this, "MainSceneCamera");
        _sceneRenderTransition = _renderCameras.create(*this, "MainSceneCameraTransition");

        for (auto i = 1; i < 4; i++)
        {
            std::stringstream ss;
            ss << "RenderCamera " << i;
            _renderCameras.create(*this, ss.str());
        }
    }
    Engine::~Engine()
    {

    }

    float Engine::spriteScale() const
    {
        return _spriteScale;
    }
    void Engine::spriteScale(float scale)
    {
        _spriteScale = scale;
    }

    float Engine::cameraScale() const
    {
        return _cameraScale;
    }
    void Engine::cameraScale(float scale)
    {
        _cameraScale = scale;
    }

    float Engine::timeScale() const
    {
        return _timeScale;
    }
    void Engine::timeScale(float scale)
    {
        _timeScale = Utils::clamp(scale, 0.0f, 2.0f);
    }

    sf::Vector2u Engine::windowSize() const
    {
        if (_headlessMode)
        {
            return sf::Vector2u(800, 600);
        }
        return _window->getSize();
    }

    sf::Vector2u Engine::renderSize() const
    {
        return _sceneRender->texture().getSize();
    }

    BaseGameScene *Engine::gameScene(std::unique_ptr<BaseGameScene> gameScene)
    {
        if (_gameScene.get())
        {
            onGameSceneEnded.emit(_gameScene.get());
        }

        _gameScene = std::move(gameScene);
        _currentSession = nullptr;

        if (_gameScene.get())
        {
            auto gameSession = dynamic_cast<GameSession *>(_gameScene.get());
            if (gameSession)
            {
                _currentSession = gameSession;
            }

            onGameSceneStarted.emit(_gameScene.get());
        }

        return _gameScene.get();
    }

    GameSession *Engine::startGameSession()
    {
        auto gameSession = std::make_unique<GameSession>(*this);
        auto result = gameSession.get();
        gameScene(std::move(gameSession));
        return result;
    }

    void Engine::changeCursor(const DefinitionId &cursorId)
    {
        if (!Mouse::cursor() || Mouse::cursor()->id != cursorId)
        {
            const Cursor *cursor;
            if (_definitionManager->tryGet(cursorId, &cursor))
            {
                Mouse::cursor(cursor);
            }
        }
    }

    void Engine::initEffects()
    {
        _bloomEffect = std::make_unique<BloomEffect>();
        _bloomEffect->init(definitionManager());

        _overlay = std::make_unique<Overlay>();
        _overlay->init(definitionManager());
    }

    void Engine::processEvents()
    {
        Keyboard::resetKeys();
        Mouse::resetButtons();

        if (_headlessMode)
        {
            return;
        }

        sf::Event event;
        while (_window->pollEvent(event))
        {
            processEvent(event);
        }
    }

    void Engine::processEvent(const sf::Event &event)
    {
        if (_initedImgui)
        {
            ImGui::SFML::ProcessEvent(event);
        }

        _gameUIManager->processEvent(event);

        if (event.type == sf::Event::Closed)
        {
            _window->close();
        }
        // catch the resize events
        else if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            sf::Vector2f area(event.size.width, event.size.height);
            onResize(area);
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            Keyboard::setKeyDown(event.key.code);
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            Keyboard::setKeyUp(event.key.code);
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            auto &mb = event.mouseButton;
            Mouse::setMouseDown(mb.button);
            Mouse::mouseDownPosition(mb.button, sf::Vector2i(mb.x, mb.y));
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            auto &mb = event.mouseButton;
            Mouse::setMouseUp(mb.button);
            Mouse::mouseUpPosition(mb.button, sf::Vector2i(mb.x, mb.y));
        }
    }

    sf::Time Engine::deltaTime() const
    {
        return _deltaTime;
    }
    sf::Time Engine::timeSinceStart() const
    {
        return _timerSinceStart.getElapsedTime();
    }
    sf::Time Engine::timeSinceStartOnUpdate() const
    {
        return _timeSinceStartOnUpdate;
    }
    sf::Time Engine::inGameTime() const
    {
        return _inGameTime;
    }

    void Engine::onResize(sf::Vector2f area)
    {
        for (auto &renderCamera : _renderCameras.objects())
        {
            renderCamera->onResize(area, _cameraScale);
        }

        _window->setView(sf::View(area * 0.5f, area));

        if (!_headlessMode && !_initedImgui)
        {
            _initedImgui = true;
            ImGui::SFML::Init(*_window, _sceneRender->texture(), true);
            auto &style = ImGui::GetStyle();
            style.WindowRounding = 0;
            style.ChildRounding = 0;
            style.FrameRounding = 0;
            style.GrabRounding = 0;
            style.PopupRounding = 0;
            style.ScrollbarRounding = 0;

            updateCursorState();
        }
    }

    void Engine::preUpdate()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - _frameStart).count();

        DrawDebug::addFrameDuration(duration);
        _frameStart = now;

        _deltaTime = _timer.getElapsedTime();
        auto timeScale = _timeScale;
        if (_currentSession && _currentSession->isInGameMenuVisible())
        {
            timeScale = 0.0f;
        }

        if (timeScale != 1.0f)
        {
            _deltaTime *= timeScale;
        }
        _timeSinceStartOnUpdate = timeSinceStart();
        _timer.restart();

        _inGameTime += _deltaTime;

        if (_initedImgui)
        {
            ImGui::SFML::Update(_cameraScale, *_window, _sceneRender->texture(), _deltaTime);
        }

        DrawDebug::glDraw = 0;
    }

    void Engine::update()
    {
        _frameCounter++;

        if (_gameScene.get() && !DrawDebug::pauseGame)
        {
            _gameScene->update(_deltaTime);
        }

        for (auto usedRenderCamera : _renderCameras.used())
        {
            usedRenderCamera->update(_deltaTime);
        }

        if (Keyboard::isKeyDown(sf::Keyboard::Tilde))
        {
            uiManager().uiDebug().toggleShow();
        }
        if (Keyboard::isKeyDown(sf::Keyboard::P))
        {
            uiManager().uiPhotoAlbum().toggleShow();
        }
        if (Keyboard::isKeyDown(sf::Keyboard::H))
        {
            DrawDebug::hideGameUI = !DrawDebug::hideGameUI;
        }

        Mouse::prevMousePosition(sf::Mouse::getPosition());
        Mouse::update(*this, _deltaTime);

        gameUIManager().preUpdate(_deltaTime);
        gameUIManager().update(_deltaTime);
    }

    void Engine::draw()
    {
        if (_headlessMode)
        {
            return;
        }

        if (DrawDebug::hideBackground)
        {
            _sceneRender->texture().clear();
        }

        // Draw main scene
        if (!DrawDebug::hideEverything && _gameScene.get())
        {
            _gameScene->draw();
        }

        if (!DrawDebug::hideGameUI)
        {
            gameUIManager().draw(*_sceneRender);
        }

        if (_initedImgui)
        {
            _uiManager->draw(*this);
            ImGui::SFML::Render(_sceneRender->texture());
        }

        if (!DrawDebug::hideGameUI)
        {
            Mouse::draw(*this, *_sceneRender);
        }

        _sceneRender->texture().display();

        // Draw from render texture to window
        finalDraw(*_sceneRender, *_window);
        _window->display();
    }

    void Engine::shutdown()
    {
        ImGui::SFML::Shutdown();
    }

    void Engine::finalDraw(sf::RenderTexture &from, sf::RenderTarget &to)
    {
        to.setActive(true);
        to.clear();
        if (enableBloom)
        {
            _bloomEffect->apply(from, to);
        }
        else
        {
            sf::Sprite sprite(from.getTexture());
            sprite.setScale(_cameraScale, _cameraScale);

            DrawDebug::glDraw++;
            to.draw(sprite);
        }
    }

    void Engine::updateCursorState()
    {
        if (_headlessMode)
        {
            return;
        }

        if (_initedImgui)
        {
            auto &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
            io.MouseDrawCursor = false;
        }
    }
}
