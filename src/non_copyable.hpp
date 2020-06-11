#pragma once

namespace space
{
    class NonCopyable
    {
        public:
            NonCopyable() = default;
            ~NonCopyable() = default;

        private:
            NonCopyable(const NonCopyable&) = delete;
            NonCopyable& operator=(const NonCopyable&) = delete;
        };
} // town