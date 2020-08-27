#pragma once

#include <stddef.h>

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

            static size_t totalMemoryAllocated;
            static size_t allocatedThisFrame;
            static size_t freedThisFrame;

            // Constructor

            // Methods
    };
} // space