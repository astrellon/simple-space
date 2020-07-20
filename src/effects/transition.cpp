#include "transition.hpp"

#include "../utils.hpp"

namespace space
{
    float Transition::percent(const sf::Time &currentTime) const
    {
        auto t = (double)(currentTime - startTime).asMicroseconds() / (double)duration.asMicroseconds();

        return Utils::clamp01(static_cast<float>(t));
    }
} // namespace space
