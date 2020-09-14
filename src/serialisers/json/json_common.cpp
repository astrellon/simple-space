#include "json_common.hpp"

#include "../../utils.hpp"

namespace space
{
    template <>
    json toJsonArray(const std::vector<std::string> &input)
    {
        json result;
        for (auto &iter : input)
        {
            result.push_back(iter);
        }
        return result;
    }

    template <>
    json toJsonArray(const std::vector<uint> &input)
    {
        json result;
        for (auto &iter : input)
        {
            result.push_back(iter);
        }
        return result;
    }

    json toJson(const PhysicsShape &input)
    {
        auto result = json {
            {"type", PhysicsShape::toString(input.type())}
        };

        if (input.type() == PhysicsShape::Circle)
        {
            result["radius"] = input.radius();
        }
        else if (input.type() == PhysicsShape::Rectangle)
        {
            result["width"] = input.width();
            result["height"] = input.height();
        }

        if (input.offset() != sf::Vector2f())
        {
            result["offset"] = toJson(input.offset());
        }

        return result;
    }
    PhysicsShape fromJsonPhysicsShape(const json &j)
    {
        PhysicsShape result;
        auto typeString = j.at("type").get<std::string>();
        result.type(PhysicsShape::fromString(typeString));

        if (result.type() == PhysicsShape::Rectangle)
        {
            result.width(j.at("width").get<float>());
            result.height(j.at("height").get<float>());
        }
        else if (result.type() == PhysicsShape::Circle)
        {
            result.radius(j.at("radius").get<float>());
        }

        auto offsetFind = j.find("offset");
        if (offsetFind != j.end())
        {
            result.offset(fromJsonVector2f(*offsetFind));
        }

        return result;
    }

    json toJson(const SpaceTransform &input)
    {
        auto result = json {
            {"position", toJson(input.position)},
        };

        if (input.rotation != 0.0f)
        {
            result["rotation"] = input.rotation;
        }
        if (input.scale != 1.0f)
        {
            result["scale"] = input.scale;
        }

        return result;
    }

    SpaceTransform fromJsonTransform(const json &j)
    {
        float scale = 1.0f, rotation = 0.0f;
        auto position = fromJsonVector2f(j.at("position"));

        Utils::json_try_set(j, "scale", scale);
        Utils::json_try_set(j, "rotation", rotation);

        return SpaceTransform(rotation, position, scale);
    }
} // namespace space
