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

#include "ui/ui_interactables.hpp"

namespace space
{
    Engine::Engine(sf::RenderWindow &window) :
        _spriteScale(1.0f), _window(window), _deltaTime(sf::Time::Zero), _timeSinceStartOnUpdate(sf::Time::Zero),
        _camera(*this), _enableBloom(true), _cameraScale(2.0f), _initedImgui(false)
    {
        _resourceManager = std::make_unique<ResourceManager>();
        _definitionManager = std::make_unique<DefinitionManager>();
    }
    Engine::~Engine()
    {

    }

    const ResourceManager &Engine::resourceManager() const
    {
        return *_resourceManager.get();
    }

    const DefinitionManager &Engine::definitionManager() const
    {
        return *_definitionManager.get();
    }

    const Camera &Engine::camera() const
    {
        return _camera;
    }

    ResourceManager &Engine::resourceManager()
    {
        return *_resourceManager.get();
    }

    DefinitionManager &Engine::definitionManager()
    {
        return *_definitionManager.get();
    }

    Camera &Engine::camera()
    {
        return _camera;
    }

    float Engine::spriteScale() const
    {
        return _spriteScale;
    }
    void Engine::spriteScale(float scale)
    {
        _spriteScale = scale;
    }

    sf::Vector2u Engine::windowSize() const
    {
        return _window.getSize();
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

    void Engine::initBackground()
    {
        sf::Shader *starShader;
        if (_resourceManager->shader("stars", &starShader))
        {
            for (auto i = 0; i < 7; i++)
            {
                _backgrounds.emplace_back(std::make_unique<StarBackground>(*this, starShader, 800, 200, 0.9f - (7 - i) * 0.1f));
            }
        }
        else
        {
            std::cout << "Unable to load shader for stars" << std::endl;
        }

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
        area *= 1.0f / _cameraScale;

        _camera.size(area);

        _sceneRenderTarget.create(area.x, area.y);

        if (!_initedImgui)
        {
            _initedImgui = true;
            ImGui::SFML::Init(_window, _sceneRenderTarget, true);
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
            ImGui::SFML::Update(_cameraScale, _window, _sceneRenderTarget, _deltaTime);
        }

        DrawDebug::glDraw = 0;
    }

    void Engine::update()
    {
        if (_currentSession.get())
        {
            _currentSession->update(_deltaTime);
        }

        _camera.update(_deltaTime);

        for (auto &b : _backgrounds)
        {
            b->update(_deltaTime);
        }

        //std::cout << "FPS: " << 1.0f / _deltaTime.asSeconds() << std::endl;
    }

    void Engine::draw()
    {
        _sceneRenderTarget.setActive(true);

        // Draw background
        _sceneRenderTarget.clear(sf::Color(20, 24, 46));
        for (auto &b : _backgrounds)
        {
            _sceneRenderTarget.setView(b->camera().view());
            b->draw(_sceneRenderTarget);
        }

        // Draw main scene
        _sceneRenderTarget.setView(_camera.view());

        if (_currentSession.get())
        {
            _currentSession->draw(_sceneRenderTarget);
        }

        if (_initedImgui)
        {
            UIInteractables::drawInteractables(*_currentSession);

            ImGui::SFML::Render(_sceneRenderTarget);
        }

        _sceneRenderTarget.display();

        // Draw from render texture to window
        _window.setActive(true);
        _window.clear();
        if (_enableBloom)
        {
            _bloomEffect.apply(_sceneRenderTarget, _window);
        }
        else
        {
            sf::Sprite sprite(_sceneRenderTarget.getTexture());
            sprite.setScale(2, 2);
            _window.draw(sprite);
        }

        _window.display();
    }

    void Engine::shutdown()
    {
        ImGui::SFML::Shutdown();
    }
}
