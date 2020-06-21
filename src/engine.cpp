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

namespace space
{
    Engine::Engine(sf::RenderWindow &window) :
        _spriteScale(1.0f), _spriteSize(16.0f), _window(window), _deltaTime(sf::Time::Zero), _timeSinceStartOnUpdate(sf::Time::Zero),
        _camera(*this), _enableBloom(true)
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

    float Engine::spriteSize() const
    {
        return _spriteSize;
    }
    void Engine::spriteSize(float size)
    {
        _spriteSize = size;
    }

    float Engine::spriteScaleCombined() const
    {
        return _spriteSize * _spriteScale;
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
        for (auto i = 0; i < 7; i++)
        {
            _backgrounds.emplace_back(std::make_unique<StarBackground>(*this, 200, 500, 0.4 - (7 - i) * 0.05));
        }

        _bloomEffect.init(*_resourceManager.get());
        _bloomEffect2.init(*_resourceManager.get());
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
        area *= 0.5f;

        _camera.setSize(area);
        for (auto &b : _backgrounds)
        {
            b->onResize(area);
        }

        _sceneRenderTarget.create(area.x, area.y);
    }

    void Engine::preUpdate()
    {
        _deltaTime = _timer.getElapsedTime();
        _timeSinceStartOnUpdate = timeSinceStart();
        _timer.restart();

        DrawDebug::glDraw = 0;
    }

    void Engine::update()
    {
        if (_currentSession.get())
        {
            _currentSession->update(_deltaTime);
        }

        for (auto &b : _backgrounds)
        {
            b->update(_deltaTime);
        }

        _camera.update(_deltaTime);

        std::cout << "FPS: " << 1.0f / _deltaTime.asSeconds() << std::endl;
    }

    void Engine::draw()
    {
        _sceneRenderTarget.setActive(true);

        _sceneRenderTarget.clear(sf::Color(20, 24, 46));
        _sceneRenderTarget.setView(_camera.view());

        _backgrounds.begin()->get()->bindShader(sf::Transform::Identity);
        for (auto &b : _backgrounds)
        {
            b->draw(_sceneRenderTarget, sf::Transform::Identity);
        }
        _backgrounds.begin()->get()->unbindShader();

        if (_currentSession.get())
        {
            _currentSession->draw(_sceneRenderTarget);
        }

        _sceneRenderTarget.display();

        _window.setActive(true);
        _window.clear();
        if (_enableBloom)
        {
            _bloomEffect2.apply(_sceneRenderTarget, _window);
        }
        else
        {
            sf::Sprite sprite(_sceneRenderTarget.getTexture());
            sprite.setScale(2, 2);
            _window.draw(sprite);
        }

        _window.display();

        auto end = std::chrono::steady_clock::now();
        // std::cout << "D = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    }
}
