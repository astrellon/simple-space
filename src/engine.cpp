#include "engine.hpp"

#include <iostream>
#include <fstream>

#include "utils.hpp"
#include "game_session.hpp"
#include "keyboard.hpp"
#include "particles.hpp"

#include <SFML/OpenGL.hpp>

namespace space
{
    Engine::Engine(sf::RenderWindow &window) :
        _spriteScale(1.0f), _spriteSize(16.0f), _window(window), _deltaTime(sf::Time::Zero), _timeSinceStartOnUpdate(sf::Time::Zero),
        _camera(*this)
    {
        _fontManager = std::make_unique<FontManager>();
        _textureManager = std::make_unique<TextureManager>();
        _definitionManager = std::make_unique<DefinitionManager>();
        _shaderManager = std::make_unique<ShaderManager>();



        for (auto i = 0; i < 7; i++)
        {
            _backgrounds.emplace_back(std::make_unique<StarBackground>(*this, 500 + i * 50, 200 - i * 10, 0.9f - i * 0.14f));
        }
    }
    Engine::~Engine()
    {

    }

    const FontManager &Engine::fontManager() const
    {
        return *_fontManager.get();
    }

    const TextureManager &Engine::textureManager() const
    {
        return *_textureManager.get();
    }

    const DefinitionManager &Engine::definitionManager() const
    {
        return *_definitionManager.get();
    }

    const ShaderManager &Engine::shaderManager() const
    {
        return *_shaderManager.get();
    }

    const Camera &Engine::camera() const
    {
        return _camera;
    }

    FontManager &Engine::fontManager()
    {
        return *_fontManager.get();
    }

    TextureManager &Engine::textureManager()
    {
        return *_textureManager.get();
    }

    DefinitionManager &Engine::definitionManager()
    {
        return *_definitionManager.get();
    }

    ShaderManager &Engine::shaderManager()
    {
        return *_shaderManager.get();
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
        _camera.setSize(area);
    }

    void Engine::preUpdate()
    {
        _deltaTime = _timer.getElapsedTime();
        _timeSinceStartOnUpdate = timeSinceStart();
        _timer.restart();
    }

    void Engine::update()
    {
        if (_currentSession.get())
        {
            _currentSession->update(_deltaTime);
        }

        for (auto &background : _backgrounds)
        {
            background->update(_deltaTime);
        }

        _camera.update(_deltaTime);
    }

    void Engine::draw()
    {
        _window.clear();
        _window.setView(_camera.view());

        for (auto &background : _backgrounds)
        {
            background->draw(_window, sf::Transform::Identity);
        }

        if (_currentSession.get())
        {
            _currentSession->draw(_window);
        }

        _window.display();
    }
}