#pragma once

#include "non_copyable.hpp"
#include "resource_manager.hpp"
#include "definition_manager.hpp"
#include "camera.hpp"
#include "effects/bloom-effect.hpp"
#include "effects/bloom-effect-2.hpp"
#include "effects/overlay.hpp"
#include "render_camera.hpp"

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;
    class UIManager;

    class Engine : private NonCopyable
    {
        public:
            Engine(sf::RenderWindow &window);
            ~Engine();

            const ResourceManager &resourceManager() const { return *_resourceManager.get(); }
            const DefinitionManager &definitionManager() const { return *_definitionManager.get(); }
            const UIManager &uiManager() const { return *_uiManager.get(); }

            ResourceManager &resourceManager() { return *_resourceManager.get(); }
            DefinitionManager &definitionManager() { return *_definitionManager.get(); }
            UIManager &uiManager() { return *_uiManager.get(); }

            Overlay &overlay() { return *_overlay; }

            float spriteScale() const;
            void spriteScale(float scale);

            float cameraScale() const;
            void cameraScale(float scale);

            sf::Vector2u windowSize() const;
            sf::Vector2u renderSize() const;

            GameSession *currentSession() const;
            GameSession *startGameSession();

            sf::Time deltaTime() const;
            sf::Time timeSinceStart() const;
            sf::Time timeSinceStartOnUpdate() const;

            RenderCamera &sceneRender() { return _sceneRender; }
            RenderCamera &sceneRenderTransition() { return _sceneRenderTransition; }

            void initEffects();

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
            std::unique_ptr<UIManager> _uiManager;

            RenderCamera _sceneRender;
            RenderCamera _sceneRenderTransition;
            BloomEffect _bloomEffect;
            BloomEffect2 _bloomEffect2;
            std::unique_ptr<Overlay> _overlay;

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
