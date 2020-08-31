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
#include "serialisers/json/json_common.hpp"
#include "PerlinNoise.hpp"

using nlohmann::json;

namespace space
{
    class Utils
    {
        public:
            static std::vector<std::string> split(const std::string &input, char delimiter, bool trimTokens);

            static std::string &rtrim(std::string &str);
            static std::string &ltrim(std::string &str);
            static std::string &trim(std::string &str);

            static inline float getInsideScale() { return 1.0f / 6.0f; }

            static std::string filenameWithoutExt(const std::string &str);
            static std::string getFilenameExt(const std::string &str);

            template <typename T>
            static inline bool remove(std::vector<T> &list, T item)
            {
                auto find = std::find(list.begin(), list.end(), item);
                if (find != list.end())
                {
                    list.erase(find);
                    return true;
                }
                return false;
            }

            template <typename T>
            static bool json_try_set(const json &j, const std::string &name, T &result)
            {
                auto find = j.find(name);
                if (find != j.end())
                {
                    find->get_to(result);
                    return true;
                }

                return false;
            }

            template <typename T>
            static bool json_try_set(const json &j, const std::string &name, sf::Vector2<T> &result)
            {
                auto find = j.find(name);
                if (find != j.end())
                {
                    result = fromJsonVector2<T>(*find);
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

            static inline float parametricBlend(float t)
            {
                auto sqt = t * t;
                return sqt / (2.0f * (sqt - t) + 1.0f);
            }

            static inline sf::Vector2f transformDirection(const sf::Vector2f &direction, const sf::Transform &transform)
            {
                const auto mats = transform.getMatrix();

                return sf::Vector2f(mats[0] * direction.x + mats[4] * direction.y,
                    mats[1] * direction.x + mats[5] * direction.y);
            }

            static inline sf::Vector2f transformPoint(const sf::Vector2f &point, const sf::Transform &transform)
            {
                const auto mats = transform.getMatrix();

                return sf::Vector2f(mats[0] * point.x + mats[4] * point.y + mats[12],
                    mats[1] * point.x + mats[5] * point.y + mats[13]);
            }

            static inline sf::Vector2f getPosition(const sf::Transform &transform)
            {
                auto mat = transform.getMatrix();
                return sf::Vector2f(mat[12], mat[13]);
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

            static inline float clamp(float input, float lower, float upper)
            {
                return input < lower ? lower : (input > upper ? upper : input);
            }
            static inline float clamp01(float input)
            {
                return input < 0 ? 0 : (input > 1 ? 1 : input);
            }

            static inline sf::Vector2f clampLength(const sf::Vector2f &input, float minLength, float maxLength)
            {
                auto len = length(input);
                if (len >= minLength && len <= maxLength)
                {
                    return input;
                }

                auto multiply = len < minLength ? minLength : maxLength;

                return sf::Vector2f(input.x / len * multiply, input.y / len * multiply);
            }

            static inline float length(const sf::Vector2f &input)
            {
                return std::sqrt(lengthSquared(input));
            }

            static inline float lengthSquared(const sf::Vector2f &input)
            {
                return dot(input, input);
            }

            static inline float dot(const sf::Vector2f &v1, const sf::Vector2f &v2)
            {
                return v1.x * v2.x + v1.y * v2.y;
            }

            static inline sf::Vector2f normalised(const sf::Vector2f &input)
            {
                auto invlen = 1.0f / length(input);
                return sf::Vector2f(input.x * invlen, input.y * invlen);
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

            static inline float perlin(float x)
            {
                return _perlinNoise.noise1D_0_1(x);
            }
            static inline float perlin(float x, float y)
            {
                return _perlinNoise.noise2D_0_1(x, y);
            }

        private:
            Utils();

            static std::random_device _rd;
            static std::mt19937 _rand;

            static siv::PerlinNoise _perlinNoise;
    };
} // town