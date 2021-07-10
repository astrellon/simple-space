#pragma once

#include "game_session.hpp"

namespace space
{
    class EditorCameraTarget;

    class EditorGameSession : public GameSession
    {
        public:
            // Fields

            // Constructor
            EditorGameSession(Engine &engine);

            // Methods
            virtual void update(sf::Time dt);
            virtual void draw();

        private:
            // Fields
            EditorCameraTarget *_cameraTarget;

            // Methods
    };
} // space