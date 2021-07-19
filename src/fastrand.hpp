#pragma once

#include <chrono>

namespace space
{
    class Fastrand
    {
        public:
            // Fields
            int seed;

            // Constructor
            Fastrand()
            {
                auto time = std::chrono::high_resolution_clock::now();
                seed = static_cast<int>(std::chrono::time_point_cast<std::chrono::microseconds>(time).time_since_epoch().count());
            }
            Fastrand(int seed) : seed(seed)
            {

            }

            // Methods
            inline int next()
            {
                seed = (214013 * seed + 2531011);
                return (seed >> 16) & 0x7FFF;
            }

            inline float next(float min, float max)
            {
                auto value = next();
                auto fvalue = static_cast<float>(value) / static_cast<float>(0x7FFF);
                auto range = max - min;
                return fvalue * range + min;
            }

    };
} // space