#pragma once

#include <functional>
#include <string>
#include <vector>
#include <random>

#include <SFML/System.hpp>

namespace space
{
    class Utils
    {
        public:
            static std::vector<std::string> split(const std::string &input, char delimiter, bool trimTokens);

            static std::string &rtrim(std::string &str);
            static std::string &ltrim(std::string &str);
            static std::string &trim(std::string &str);

            static std::string filename_without_ext(const std::string &str);

            typedef std::function<bool (const std::string &)> LineReader;
            static void readCSVLines(const std::string &filename, LineReader lineReader);

            static inline float randf(float min = 0.0f, float max = 1.0f)
            {
                std::uniform_real_distribution<float> dist(min, max);
                return dist(_rand);
            }
            static inline int randi(int min = 0, int max = 1)
            {
                auto floatValue = randf(static_cast<float>(min), static_cast<float>(max));

                return static_cast<int>(floatValue);
            }

            static inline float lerp(float init, float target, float t)
            {
                return (target - init) * t + init;
            }

            static inline sf::Vector2f lerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
            {
                auto x = lerp(init.x, target.x, t);
                auto y = lerp(init.y, target.y, t);
                return sf::Vector2f(x, y);
            }

            static inline float para_lerp(float init, float target, float t)
            {
                auto x = (t - 1);
                x = -x*x + 1;
                return (target - init) * x + init;
            }
            static inline sf::Vector2f para_lerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
            {
                auto x = para_lerp(init.x, target.x, t);
                auto y = para_lerp(init.y, target.y, t);
                return sf::Vector2f(x, y);
            }

        private:
            Utils();

            static std::random_device _rd;
            static std::mt19937 _rand;

            public:
    };
} // town