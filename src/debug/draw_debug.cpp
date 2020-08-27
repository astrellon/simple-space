#include "draw_debug.hpp"

namespace space
{
    int DrawDebug::glDraw = 0;
    bool DrawDebug::slow = false;
    bool DrawDebug::showPolygons = false;
    bool DrawDebug::showUIWindow = false;
    bool DrawDebug::hideGameSession = false;

    size_t DrawDebug::totalMemoryAllocated = 0;
    size_t DrawDebug::allocatedThisFrame = 0;
    size_t DrawDebug::freedThisFrame = 0;
} // namespace space
