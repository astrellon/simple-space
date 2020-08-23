#pragma once

#include "json.hpp"

#include <SFML/System.hpp>

#include <functional>
#include <memory>
#include <string>

#include "../../physics/physics_shape.hpp"
#include "../../space_transform.hpp"

using nlohmann::json;

namespace space
{
    template<typename T>
    inline json toJson(const sf::Vector2<T> &input)
    {
        return json { input.x, input.y };
    }

    template<typename T>
    inline sf::Vector2<T> fromJsonVector2(const json &j)
    {
        auto x = j[0].get<T>();
        auto y = j[1].get<T>();

        return sf::Vector2<T>(x, y);
    }

    inline sf::Vector2f fromJsonVector2f(const json &j)
    {
        return fromJsonVector2<float>(j);
    }
    inline sf::Vector2i fromJsonVector2i(const json &j)
    {
        return fromJsonVector2<int>(j);
    }
    inline sf::Vector2u fromJsonVector2u(const json &j)
    {
        return fromJsonVector2<unsigned int>(j);
    }

    template <typename T>
    json toJsonArray(const std::vector<T> &input)
    {
        json result;
        for (auto &iter : input)
        {
            result.push_back(toJson(iter));
        }
        return result;
    }

    template <typename T>
    json toJsonArray(const std::vector<std::unique_ptr<T>> &input)
    {
        json result;
        for (auto &iter : input)
        {
            auto j = toJson(*iter.get());

            if (!j.is_null())
                result.push_back(j);
        }
        return result;
    }

    template <>
    json toJsonArray(const std::vector<std::string> &input);

    template <>
    json toJsonArray(const std::vector<uint> &input);

    json toJson(const PhysicsShape &input);
    PhysicsShape fromJsonPhysicsShape(const json &j);

    json toJson(const SpaceTransform &input);
    SpaceTransform fromJsonTransform(const json &j);

} // namespace space
