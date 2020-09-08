#pragma once

#include <stddef.h>
#include <array>

namespace space
{
    class DrawDebug
    {
        public:
            // Fields
            static int glDraw;
            static bool slow;
            static bool showPolygons;
            static bool showUIWindow;
            static bool hideGameSession;
            static bool hideEverything;
            static bool showPortalShapes;

            static size_t totalMemoryAllocated;
            static size_t allocatedThisFrame;
            static size_t freedThisFrame;
            static size_t numAllocations;
            static size_t locksUsed;
            static std::array<float, 1000> frameDurations;

            // Constructor

            // Methods
            static void addFrameDuration(size_t frameDuration);
            static float averageFrameDuration();

        private:
            static size_t frameDurationIndex;
    };
} // space