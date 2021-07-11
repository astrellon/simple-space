#pragma once

#include <SFML/Graphics.hpp>

#include "game_session.hpp"

namespace space
{
    class EditorCameraTarget;
    class Area;

    class EditorGameSession : public GameSession
    {
        public:
            // Fields

            // Constructor
            EditorGameSession(Engine &engine);

            // Methods
            virtual void update(sf::Time dt);
            virtual void draw();
            virtual void onPostLoad(LoadingContext &context);

            void moveCameraTo(Area &area, sf::Vector2f position = sf::Vector2f());

        private:
            // Fields
            EditorCameraTarget *_cameraTarget;

            // Methods
    };
} // space