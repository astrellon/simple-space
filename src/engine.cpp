#include "engine.hpp"

#include <iostream>
#include <fstream>
#include <chrono>

#include "utils.hpp"
#include "game_session.hpp"
#include "keyboard.hpp"
#include "particles.hpp"
#include "debug/draw_debug.hpp"

#include <SFML/OpenGL.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "game/items/placed_item.hpp"
#include "game/items/placeable_item.hpp"
#include "game/ship.hpp"

#include "ui/ui_manager.hpp"

namespace space
{
    Engine::Engine(sf::RenderWindow &window) :
        _spriteScale(1.0f), _window(window), _deltaTime(sf::Time::Zero), _timeSinceStartOnUpdate(sf::Time::Zero),
        _enableBloom(true), _cameraScale(2.0f), _initedImgui(false), _sceneRender(*this, "SceneRender"), _sceneRenderTransition(*this, "SceneRenderTrans")
    {
        _resourceManager = std::make_unique<ResourceManager>();
        _definitionManager = std::make_unique<DefinitionManager>();
        _uiManager = std::make_unique<UIManager>();
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
        return _window.getSize();
    }

    sf::Vector2u Engine::renderSize() const
    {
        return _sceneRender.texture().getSize();
    }

    GameSession *Engine::currentSession() const
    {
        return _currentSession.get();
    }
    GameSession *Engine::startGameSession()
    {
        _currentSession = std::make_unique<GameSession>(*this);
        return _currentSession.get();
    }

    void Engine::initEffects()
    {
        _bloomEffect.init(*_resourceManager.get());
        _bloomEffect2.init(*_resourceManager.get());

        _overlay = std::make_unique<Overlay>(*this);
    }

    void Engine::processEvents()
    {
        sf::Event event;
        Keyboard::resetKeys();

        while (_window.pollEvent(event))
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
            _window.close();
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
        _sceneRender.onResize(area, _cameraScale);
        _sceneRenderTransition.onResize(area, _cameraScale);

        if (!_initedImgui)
        {
            _initedImgui = true;
            ImGui::SFML::Init(_window, _sceneRender.texture(), true);
            auto &style = ImGui::GetStyle();
            style.WindowRounding = 0;
            style.ChildRounding = 0;
            style.FrameRounding = 0;
            style.GrabRounding = 0;
            style.PopupRounding = 0;
            style.ScrollbarRounding = 0;
        }
    }

    void Engine::preUpdate()
    {
        _deltaTime = _timer.getElapsedTime();
        _timeSinceStartOnUpdate = timeSinceStart();
        _timer.restart();

        if (_initedImgui)
        {
            ImGui::SFML::Update(_cameraScale, _window, _sceneRender.texture(), _deltaTime);
        }

        DrawDebug::glDraw = 0;
    }

    void Engine::update()
    {
        if (_currentSession.get())
        {
            _currentSession->update(_deltaTime);
        }

        _sceneRender.update(_deltaTime);
        _sceneRenderTransition.update(_deltaTime);
    }

    void Engine::draw()
    {
        // Draw main scene
        if (_currentSession.get())
        {
            _currentSession->draw();
        }

        if (_initedImgui)
        {
            _uiManager->draw(*this);
            if (_currentSession.get())
            {
                _currentSession->drawUI(_sceneRender.texture());
            }

            ImGui::SFML::Render(_sceneRender.texture());
        }

        _sceneRender.texture().display();

        // Draw from render texture to window
        _window.setActive(true);
        _window.clear();
        if (_enableBloom)
        {
            _bloomEffect.apply(_sceneRender.texture(), _window);
        }
        else
        {
            sf::Sprite sprite(_sceneRender.texture().getTexture());
            sprite.setScale(_cameraScale, _cameraScale);
            _window.draw(sprite);
        }

        _window.display();
    }

    void Engine::shutdown()
    {
        ImGui::SFML::Shutdown();
    }
}
