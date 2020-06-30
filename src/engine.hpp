#pragma once

#include "non_copyable.hpp"
#include "resource_manager.hpp"
#include "definition_manager.hpp"
#include "camera.hpp"
#include "star_background.hpp"
#include "effects/bloom-effect.hpp"
#include "effects/bloom-effect-2.hpp"

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;

    class Engine : private NonCopyable
    {
        public:
            Engine(sf::RenderWindow &window);
            ~Engine();

            const ResourceManager &resourceManager() const;
            const DefinitionManager &definitionManager() const;
            const Camera &camera() const;

            ResourceManager &resourceManager();
            DefinitionManager &definitionManager();
            Camera &camera();

            float spriteScale() const;
            void spriteScale(float scale);

            sf::Vector2u windowSize() const;

            GameSession *currentSession() const;
            GameSession *startGameSession();

            sf::Time deltaTime() const;
            sf::Time timeSinceStart() const;
            sf::Time timeSinceStartOnUpdate() const;

            void initBackground();

            void onResize(sf::Vector2f area);
            void processEvents();
            void processEvent(const sf::Event &event);
            void preUpdate();
            void update();
            void draw();
            void shutdown();

        private:
            std::unique_ptr<ResourceManager> _resourceManager;
            std::unique_ptr<DefinitionManager> _definitionManager;

            std::vector<std::unique_ptr<StarBackground>> _backgrounds;
            sf::RenderTexture _sceneRenderTarget;
            BloomEffect _bloomEffect;
            BloomEffect2 _bloomEffect2;

            Camera _camera;

            float _spriteScale;
            float _cameraScale;
            bool _initedImgui;
            std::unique_ptr<GameSession> _currentSession;
            sf::RenderWindow &_window;
            sf::Clock _timer;
            sf::Clock _timerSinceStart;
            sf::Time _deltaTime;
            sf::Time _timeSinceStartOnUpdate;

            bool _enableBloom;
    };
} // town
