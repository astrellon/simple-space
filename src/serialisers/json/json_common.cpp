#include "json_common.hpp"

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
        return json {
            {"rotation", input.rotation},
            {"position", toJson(input.position)},
            {"scale", input.scale}
        };
    }
    SpaceTransform fromJsonTransform(const json &j)
    {
        auto rotation = j.at("rotation").get<float>();
        auto position = fromJsonVector2f(j.at("position"));
        auto scale = j.at("scale").get<float>();

        return SpaceTransform(rotation, position, scale);
    }
} // namespace space
