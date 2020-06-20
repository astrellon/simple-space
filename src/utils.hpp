#pragma once

#include <functional>
#include <string>
#include <vector>
#include <random>

#include <SFML/Graphics.hpp>
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

            static std::string filenameWithoutExt(const std::string &str);
            static std::string getFilenameExt(const std::string &str);

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

            static std::mt19937 randWithSeed(int seed);

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

            static inline float paraLerp(float init, float target, float t)
            {
                auto x = (t - 1);
                x = -x*x + 1;
                return (target - init) * x + init;
            }
            static inline sf::Vector2f paraLerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
            {
                auto x = paraLerp(init.x, target.x, t);
                auto y = paraLerp(init.y, target.y, t);
                return sf::Vector2f(x, y);
            }

            static inline sf::Vector2f transformDirection(const sf::Vector2f &direction, const sf::Transform &transform)
            {
                const auto mats = transform.getMatrix();

                return sf::Vector2f(mats[0] * direction.x + mats[4] * direction.y,
                    mats[1] * direction.x + mats[5] * direction.y);
            }

            static sf::Color hsv(float hue, float saturation, float value);

            static inline sf::Vector2i floor(const sf::Vector2f & input)
            {
                return sf::Vector2i(std::floor(input.x), std::floor(input.y));
            }

            static inline sf::Vector2i ceil(const sf::Vector2f & input)
            {
                return sf::Vector2i(std::ceil(input.x), std::ceil(input.y));
            }

            static inline void setColour(sf::Color &target, const sf::Color &src)
            {
                target.r = src.r;
                target.g = src.g;
                target.b = src.b;
                target.a = src.a;
            }

        private:
            Utils();

            static std::random_device _rd;
            static std::mt19937 _rand;

            public:
    };
} // town