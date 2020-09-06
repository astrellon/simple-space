#include "draw_debug.hpp"

namespace space
{
    int DrawDebug::glDraw = 0;
    bool DrawDebug::slow = false;
    bool DrawDebug::showPolygons = false;
    bool DrawDebug::showUIWindow = false;
    bool DrawDebug::hideGameSession = false;
    bool DrawDebug::hideEverything = false;

    size_t DrawDebug::totalMemoryAllocated = 0;
    size_t DrawDebug::allocatedThisFrame = 0;
    size_t DrawDebug::freedThisFrame = 0;
    size_t DrawDebug::numAllocations = 0;
    size_t DrawDebug::locksUsed = 0;
    std::array<float, 1000> DrawDebug::frameDurations;
    size_t DrawDebug::frameDurationIndex = 0;

    void DrawDebug::addFrameDuration(size_t frameDuration)
    {
        frameDurations[frameDurationIndex++] = static_cast<float>(frameDuration);
        if (frameDurationIndex >= frameDurations.size())
        {
            frameDurationIndex = 0;
        }
    }

    float DrawDebug::averageFrameDuration()
    {
        auto total = 0.0f;
        for (auto d : frameDurations)
        {
            total += d;
        }

        return total / static_cast<float>(frameDurations.size());
    }
} // namespace space
