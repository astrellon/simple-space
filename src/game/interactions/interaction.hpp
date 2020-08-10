#pragma once

#include <string>

#include "../../non_copyable.hpp"

namespace space
{
    class GameSession;

    class Interaction : private NonCopyable
    {
        public:
            // Methods
            virtual std::string label() const = 0;
            virtual void execute(GameSession &session) = 0;
    };
} // space