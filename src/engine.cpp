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

#include "effects/overlay.hpp"
#include "effects/bloom_effect.hpp"

#include "ui/ui_manager.hpp"
#include "ui/ui_debug.hpp"

namespace space
{
    Engine::Engine(sf::RenderWindow *window) :
        _spriteScale(1.0f), _window(window), _deltaTime(sf::Time::Zero), _timeSinceStartOnUpdate(sf::Time::Zero),
        enableBloom(true), _cameraScale(2.0f), _initedImgui(false), _sceneRender(nullptr), _sceneRenderTransition(nullptr), _headlessMode(window == nullptr)
    {
        _resourceManager = std::make_unique<ResourceManager>();
        _definitionManager = std::make_unique<DefinitionManager>();
        _uiManager = std::make_unique<UIManager>();

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

    GameSession *Engine::currentSession() const
    {
        return _currentSession.get();
    }
    void Engine::currentSession(std::unique_ptr<GameSession> session)
    {
        _currentSession = std::move(session);
        auto &player = _currentSession->playerController();
        if (player.controlling() == space::ControlShip)
        {
            _currentSession->setPlayerControllingShip(player.controllingShip());
        }
        else if (player.controlling() == space::ControlCharacter)
        {
            _currentSession->setPlayerControllingCharacter();
        }
    }
    GameSession *Engine::startGameSession()
    {
        _currentSession = std::make_unique<GameSession>(*this);
        return _currentSession.get();
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

    void Engine::onResize(sf::Vector2f area)
    {
        for (auto &renderCamera : _renderCameras.objects())
        {
            renderCamera->onResize(area, _cameraScale);
        }

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

            auto &io = ImGui::GetIO();
            io.MouseDrawCursor = true;
        }
    }

    void Engine::preUpdate()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - _frameStart).count();

        DrawDebug::addFrameDuration(duration);
        _frameStart = now;

        _deltaTime = _timer.getElapsedTime();
        _timeSinceStartOnUpdate = timeSinceStart();
        _timer.restart();

        if (_initedImgui)
        {
            ImGui::SFML::Update(_cameraScale, *_window, _sceneRender->texture(), _deltaTime);
        }

        //ImGui::IsMouseHoveringRect()

        DrawDebug::glDraw = 0;
    }

    void Engine::update()
    {
        if (_currentSession.get())
        {
            _currentSession->update(_deltaTime);
        }

        for (auto usedRenderCamera : _renderCameras.used())
        {
            usedRenderCamera->update(_deltaTime);
        }

        if (Keyboard::isKeyDown(sf::Keyboard::Tilde))
        {
            uiManager().uiDebug().show = !uiManager().uiDebug().show;
        }

        Mouse::prevMousePosition(sf::Mouse::getPosition());
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
        if (!DrawDebug::hideEverything && _currentSession.get())
        {
            _currentSession->draw();
        }

        if (_initedImgui)
        {
            _uiManager->draw(*this);
            ImGui::SFML::Render(_sceneRender->texture());
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
}
