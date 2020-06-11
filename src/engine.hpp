#pragma once

#include "non_copyable.hpp"
#include "font_manager.hpp"
#include "texture_manager.hpp"

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;

    class Engine : private NonCopyable
    {
        public:
            Engine(sf::RenderWindow &window);
            ~Engine();

            const FontManager *fontManager() const;
            const TextureManager *textureManager() const;

            FontManager *fontManager();
            TextureManager *textureManager();

            float spriteScale() const;
            void spriteScale(float scale);

            float spriteSize() const;
            void spriteSize(float size);

            float spriteScaleCombined() const;

            sf::Vector2u windowSize() const;

            GameSession *currentSession() const;
            GameSession *startGameSession();

            void processEvents();
            void processEvent(const sf::Event &event);

            sf::Time deltaTime() const;
            sf::Time timeSinceStart() const;

            void preUpdate();
            void update();
            void draw();

        private:
            std::unique_ptr<FontManager> _fontManager;
            std::unique_ptr<TextureManager> _textureManager;

            float _spriteScale;
            float _spriteSize;
            std::unique_ptr<GameSession> _currentSession;
            sf::RenderWindow &_window;
            sf::Clock _timer;
            sf::Clock _timerSinceStart;
            sf::Time _deltaTime;
    };
} // town