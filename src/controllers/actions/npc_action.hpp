#pragma once

#include <string>

#include <SFML/System.hpp>

#include "../../non_copyable.hpp"

namespace space
{
    class NpcController;

    class NpcAction : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            NpcAction (NpcController *controller);
            virtual ~NpcAction();

            // Methods
            virtual std::string type() const = 0;
            virtual void update(sf::Time dt) = 0;
            virtual bool isComplete() const = 0;

            virtual void onComplete() { }

            inline NpcController *controller() const { return _controller; }

        private:
            // Fields
            NpcController *_controller;

            // Methods
    };
} // space