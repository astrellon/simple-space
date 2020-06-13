#pragma once

#include "non_copyable.hpp"
#include "font_manager.hpp"
#include "texture_manager.hpp"
#include "definition_manager.hpp"
#include "shader_manager.hpp"
#include "camera.hpp"
#include "particles.hpp"

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;

    class Engine : private NonCopyable
    {
        public:
            Engine(sf::RenderWindow &window);
            ~Engine();

            const FontManager &fontManager() const;
            const TextureManager &textureManager() const;
            const DefinitionManager &definitionManager() const;
            const ShaderManager &shaderManager() const;
            const Camera &camera() const;

            FontManager &fontManager();
            TextureManager &textureManager();
            DefinitionManager &definitionManager();
            ShaderManager &shaderManager();
            Camera &camera();

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

            void onResize(sf::Vector2f area);

            void preUpdate();
            void update();
            void draw();

        private:
            std::unique_ptr<FontManager> _fontManager;
            std::unique_ptr<TextureManager> _textureManager;
            std::unique_ptr<DefinitionManager> _definitionManager;
            std::unique_ptr<ShaderManager> _shaderManager;

            Particles _particles;

            Camera _camera;

            float _spriteScale;
            float _spriteSize;
            std::unique_ptr<GameSession> _currentSession;
            sf::RenderWindow &_window;
            sf::Clock _timer;
            sf::Clock _timerSinceStart;
            sf::Time _deltaTime;
    };
} // town