#include "draw_debug.hpp"

namespace space
{
    int DrawDebug::glDraw = 0;
    bool DrawDebug::pauseGame = false;
    bool DrawDebug::slow = false;
    bool DrawDebug::showPolygons = false;
    bool DrawDebug::hideGameSession = false;
    bool DrawDebug::hideEverything = false;
    bool DrawDebug::hideBackground = false;
    bool DrawDebug::hideGameUI = false;
    bool DrawDebug::showPortalShapes = false;
    SpaceObject *DrawDebug::focusOnObject = nullptr;
    UIElement *DrawDebug::highlightElement = nullptr;

    size_t DrawDebug::totalMemoryAllocated = 0;
    size_t DrawDebug::allocatedThisFrame = 0;
    size_t DrawDebug::freedThisFrame = 0;
    size_t DrawDebug::numAllocations = 0;
    size_t DrawDebug::locksUsed = 0;
    std::array<float, 1000> DrawDebug::frameDurations;
    size_t DrawDebug::frameDurationIndex = 0;
    int DrawDebug::fontSize = 16;
    float DrawDebug::fov = 0.4f;
    float DrawDebug::drawClose = 0.1f;
    float DrawDebug::drawFar = 2000.0f;

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
