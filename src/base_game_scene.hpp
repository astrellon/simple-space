#pragma once

#include "non_copyable.hpp"

namespace space
{
    class BaseGameScene : private NonCopyable
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual void update(sf::Time dt) = 0;
            virtual void draw() = 0;
    };
} // space