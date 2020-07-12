#pragma once

#include <functional>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game/space_object.hpp"
#include "game/items/item.hpp"

#include "serialisers/json/json.hpp"

namespace space
{
    class Utils
    {
        public:
            static std::vector<std::string> split(const std::string &input, char delimiter, bool trimTokens);

            static std::string &rtrim(std::string &str);
            static std::string &ltrim(std::string &str);
            static std::string &trim(std::string &str);

            static inline float getInsideScale() { return 1.0f / 4.0f; }

            static std::string filenameWithoutExt(const std::string &str);
            static std::string getFilenameExt(const std::string &str);

            template <typename T>
            static inline bool json_try_set(const nlohmann::json &j, const std::string &name, T &result)
            {
                auto find = j.find(name);
                if (find != j.end())
                {
                    j.get_to(result);
                    return true;
                }

                return false;
            }

            static inline ObjectId makeObjectId(ItemId item)
            {
                std::stringstream ss;
                ss << "ITEM_" << item;
                return ss.str();
            }

            static inline float degreesToRadians(float input)
            {
                return input / 180.0f * M_PI;
            }
            static inline float radiansToDegrees(float input)
            {
                return input * 180.0f / M_PI;
            }

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

            static inline std::string toHexString(const sf::Color &input)
            {
                std::stringstream ss;
                ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << input.r << input.g << input.b << input.a;

                return ss.str();
            }

            static inline sf::Color fromHexString(const std::string &input)
            {
                std::stringstream ss(input);
                char c1, c2;
                ss >> c1 >> c2;
                std::uint32_t full;
                ss >> std::hex >> full;

                sf::Uint8 r = (full >> 24) & 0xFF;
                sf::Uint8 g = (full >> 16) & 0xFF;
                sf::Uint8 b = (full >> 8) & 0xFF;
                sf::Uint8 a = full & 0xFF;

                return sf::Color(r, g, b, a);
            }

        private:
            Utils();

            static std::random_device _rd;
            static std::mt19937 _rand;

            public:
    };
} // town