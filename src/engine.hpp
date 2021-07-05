#pragma once

#include "non_copyable.hpp"
#include "resource_manager.hpp"
#include "definition_manager.hpp"
#include "camera.hpp"
#include "render_camera.hpp"
#include "limited_object_pool.hpp"

#include <chrono>
#include <SFML/Graphics.hpp>
#include <observe/event.h>

namespace space
{
    class GameSession;
    class UIManager;
    class Overlay;
    class BloomEffect;
    class GameUIManager;
    class BaseGameScene;

    class Engine : private NonCopyable
    {
        public:
            // Types
            typedef observe::Event<BaseGameScene *> GameSceneChanged;

            // Fields
            bool enableBloom;
            GameSceneChanged onGameSceneStarted;
            GameSceneChanged onGameSceneEnded;

            // Constructor
            Engine(sf::RenderWindow *window);
            ~Engine();

            // Methods
            const ResourceManager &resourceManager() const { return *_resourceManager.get(); }
            const DefinitionManager &definitionManager() const { return *_definitionManager.get(); }
            const UIManager &uiManager() const { return *_uiManager.get(); }
            const GameUIManager &gameUIManager() const { return *_gameUIManager.get(); }

            ResourceManager &resourceManager() { return *_resourceManager.get(); }
            DefinitionManager &definitionManager() { return *_definitionManager.get(); }
            UIManager &uiManager() { return *_uiManager.get(); }
            GameUIManager &gameUIManager() { return *_gameUIManager.get(); }

            Overlay &overlay() { return *_overlay; }

            float spriteScale() const;
            void spriteScale(float scale);

            float cameraScale() const;
            void cameraScale(float scale);

            bool isHeadless() const { return _headlessMode; }

            sf::Vector2u windowSize() const;
            sf::Vector2u renderSize() const;

            GameSession *currentSession() const {  return _currentSession; }
            GameSession *startGameSession();

            BaseGameScene *gameScene() const { return _gameScene.get(); }
            BaseGameScene *gameScene(std::unique_ptr<BaseGameScene> session);

            sf::Time deltaTime() const;
            sf::Time timeSinceStart() const;
            sf::Time timeSinceStartOnUpdate() const;

            RenderCamera &sceneRender() { return *_sceneRender; }
            RenderCamera &sceneRenderTransition() { return *_sceneRenderTransition; }

            sf::RenderWindow *window() { return _window; }

            LimitedObjectPool<RenderCamera> &renderCameras() { return _renderCameras; }

            void changeCursor(const DefinitionId &cursorId);

            void initEffects();

            void onResize(sf::Vector2f area);
            void processEvents();
            void processEvent(const sf::Event &event);
            void preUpdate();
            void update();
            void draw();
            void shutdown();

            int frameCounter() const { return _frameCounter; }

        private:
            // Fields
            std::unique_ptr<ResourceManager> _resourceManager;
            std::unique_ptr<DefinitionManager> _definitionManager;
            std::unique_ptr<UIManager> _uiManager;
            std::unique_ptr<GameUIManager> _gameUIManager;

            RenderCamera *_sceneRender;
            RenderCamera *_sceneRenderTransition;
            LimitedObjectPool<RenderCamera> _renderCameras;
            std::unique_ptr<BloomEffect> _bloomEffect;
            std::unique_ptr<Overlay> _overlay;

            float _spriteScale;
            float _cameraScale;
            bool _initedImgui;
            bool _headlessMode;
            std::unique_ptr<BaseGameScene> _gameScene;
            GameSession *_currentSession;
            sf::RenderWindow *_window;
            sf::Clock _timer;
            sf::Clock _timerSinceStart;
            sf::Time _deltaTime;
            sf::Time _timeSinceStartOnUpdate;
            int _frameCounter;

            std::chrono::system_clock::time_point _frameStart;

            // Methods
            void finalDraw(sf::RenderTexture &from, sf::RenderTarget &to);

            void updateCursorState();
    };
} // town
