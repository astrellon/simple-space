#pragma once

#include "json.hpp"

#include <SFML/System.hpp>

#include <functional>
#include <memory>
#include <string>

#include "../../physics/physics_shape.hpp"

using nlohmann::json;

namespace space
{
    template<typename T>
    json toJson(const sf::Vector2<T> &input)
    {
        return json { input.x, input.y };
    }

    template<typename T>
    sf::Vector2<T> fromJsonVector2(const json &j)
    {
        auto x = j[0].get<T>();
        auto y = j[1].get<T>();

        return sf::Vector2<T>(x, y);
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
            result.push_back(toJson(*iter.get()));
        }
        return result;
    }

    template <>
    json toJsonArray(const std::vector<std::string> &input);

    json toJson(const PhysicsShape &input);
    PhysicsShape fromJsonPhysicsShape(const json &j);

} // namespace space
