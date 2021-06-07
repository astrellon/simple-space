#pragma once

#include <SFML/System.hpp>

namespace space
{
    class NPCController;

    class NPCAction
    {
        public:
            // Fields

            // Constructor
            NPCAction (NPCController *controller);
            virtual ~NPCAction();

            // Methods
            virtual void update(sf::Time dt) = 0;

            virtual bool isComplete() const = 0;

        private:
            // Fields
            NPCController *_controller;

            // Methods
    };
} // space