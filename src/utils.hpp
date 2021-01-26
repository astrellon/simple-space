#pragma once

#include <functional>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game/items/item.hpp"

#include "serialisers/json/json.hpp"
#include "serialisers/json/json_common.hpp"
#include "PerlinNoise.hpp"
#include "earcut.hpp"
#include "physics/polygon.hpp"
#include "types.hpp"

using nlohmann::json;

namespace space
{
    class Utils
    {
        public:
            static const float InsideScale;
            static const float PhysicsScale;

            static std::vector<std::string> split(const std::string &input, char delimiter, bool trimTokens);

            static std::string &rtrim(std::string &str);
            static std::string &ltrim(std::string &str);
            static std::string &trim(std::string &str);

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
            static inline bool remove(std::vector<std::unique_ptr<T>> &list, T *item)
            {
                for (auto iter = list.begin(); iter != list.end(); ++iter)
                {
                    if (iter->get() == item)
                    {
                        list.erase(iter);
                        return true;
                    }
                }

                return false;
            }

            template <typename T>
            static inline bool contains(const std::vector<T> &list, T item)
            {
                auto find = std::find(list.begin(), list.end(), item);
                return find != list.end();
            }

            template <typename T>
            static bool json_try_get(const json &j, const std::string &name, T &result)
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
            static bool json_try_get(const json &j, const std::string &name, sf::Vector2<T> &result)
            {
                auto find = j.find(name);
                if (find != j.end())
                {
                    result = fromJsonVector2<T>(*find);
                    return true;
                }

                return false;
            }

            static inline ObjectId makeItemId(ItemId item)
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
                int value = input.r << 24 | input.g << 16 | input.b << 8 | input.a;
                ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << value;

                auto result = ss.str();

                return result;
            }

            static inline sf::Color fromHexString(const std::string &input)
            {
                if (input.length() < 6)
                {
                    std::cout << "Invalid hex colour string: " << input << std::endl;
                    return sf::Color::Black;
                }

                std::stringstream ss(input);
                if (input[0] == '0' && (input[1] == 'x' || input[1] == 'X'))
                {
                    // Skip 0x
                    char c1, c2;
                    ss >> c1 >> c2;
                }

                std::uint32_t full;
                ss >> std::hex >> full;

                sf::Uint8 r = (full >> 24) & 0xFF;
                sf::Uint8 g = (full >> 16) & 0xFF;
                sf::Uint8 b = (full >> 8) & 0xFF;
                sf::Uint8 a = full & 0xFF;

                return sf::Color(r, g, b, a);
            }

            static inline sf::Vector2f radianVector(float radians, float distance)
            {
                auto x = std::cos(radians);
                auto y = std::sin(radians);

                return sf::Vector2f(x * distance, y * distance);
            }

            static inline sf::Vector2f degreeVector(float degrees, float distance)
            {
                auto x = std::cos(degreesToRadians(degrees));
                auto y = std::sin(degreesToRadians(degrees));

                return sf::Vector2f(x * distance, y * distance);
            }

            static inline float perlin(float x)
            {
                return _perlinNoise.noise1D_0_1(x);
            }
            static inline float perlin(float x, float y)
            {
                return _perlinNoise.noise2D_0_1(x, y);
            }

            static inline bool iequals(const std::string& a, const std::string& b)
            {
                return std::equal(a.begin(), a.end(),
                    b.begin(), b.end(),
                    [](char a, char b) {
                        return tolower(a) == tolower(b);
                    });
            }

            static bool tryGetIntersection(const sf::Vector2f &origin, const sf::Vector2f &direction, const sf::Vector2f &point1, const sf::Vector2f &point2, sf::Vector2f *result);

            static bool checkIfLinesIntersect(const sf::Vector2f &start1, const sf::Vector2f &end1, const sf::Vector2f &start2, const sf::Vector2f &end2);

            static inline float triangleArea(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3)
            {
                return std::abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) * 0.5);
            }

            static inline bool checkIfInsideTriangle(const sf::Vector2f &point, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3)
            {
                // Calculate area of triangle ABC
                auto A = triangleArea(p1, p2, p3);

                // Calculate area of triangle PBC
                auto A1 = triangleArea(point, p2, p3);

                // Calculate area of triangle PAC
                auto A2 = triangleArea(p1, point, p3);

                // Calculate area of triangle PAB
                auto A3 = triangleArea(p1, p2, point);

                // Check if sum of A1, A2 and A3 is same as A
                return std::abs(A - (A1 + A2 + A3)) < 0.1f;
            }

            template <typename T>
            static int checkIfInsidePolygon(const sf::Vector2f &point, const Polygon &polygon, const mapbox::detail::Earcut<T> &earcut)
            {
                auto triangleIndex = 0;
                for (auto i = 0; i < earcut.indices.size(); i += 3)
                {
                    auto &p1 = polygon[earcut.indices[i    ]];
                    auto &p2 = polygon[earcut.indices[i + 1]];
                    auto &p3 = polygon[earcut.indices[i + 2]];

                    if (checkIfInsideTriangle(point, p1, p2, p3))
                    {
                        return triangleIndex;
                    }

                    triangleIndex++;
                }

                return -1;
            }

        private:
            Utils();

            static std::random_device _rd;
            static std::mt19937 _rand;

            static siv::PerlinNoise _perlinNoise;

            static float counterClockWise(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3);
    };
} // town