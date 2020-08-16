#include "json_serialisers_game.hpp"

#include "../../game_session.hpp"

#include "../../game/space_object.hpp"
#include "../../game/character.hpp"
#include "../../game/ship.hpp"
#include "../../game/planet.hpp"
#include "../../game/planet_surface.hpp"
#include "../../game/items/placed_item.hpp"
#include "../../game/items/placeable_item.hpp"
#include "../../game/walkable_area.hpp"
#include "../../space_transform.hpp"

#include "json_common.hpp"

namespace space
{
    json toJsonPlanetId(PlanetSurface *input)
    {
        return json {input->definition.id};
    }

    json toJson(const GameSession &input)
    {
        auto result = json {
            {"spaceObjects", toJsonArray(input.spaceObjects())}
        };

        return result;
    }

    std::unique_ptr<GameSession> fromJsonGameSession(Engine &engine, const json &j)
    {
        return std::make_unique<GameSession>(engine);
    }

    json toJson(const SpaceObject &input)
    {
        if (input.type() == Character::SpaceObjectType())
        {
            return toJson(dynamic_cast<const Character &>(input));
        }
        else if (input.type() == Ship::SpaceObjectType())
        {
            return toJson(dynamic_cast<const Ship &>(input));
        }
        else if (input.type() == Planet::SpaceObjectType())
        {
            return toJson(dynamic_cast<const Planet &>(input));
        }
        else if (input.type() == PlacedItem::SpaceObjectType())
        {
            return toJson(dynamic_cast<const PlacedItem &>(input));
        }

        throw std::runtime_error("Unknown space object type");
    }

    json toJson(const Character &input)
    {
        return json {
            {"type", input.type()},
            {"id", input.id},
            {"transform", toJson(input.transform())},
        };
    }

    json toJson(const Ship &input)
    {
        return json {
            {"type", input.type()},
            {"id", input.id},
            {"transform", toJson(input.transform())},
            {"rotationSpeed", input.rotationSpeed()},
            {"speed", toJson(input.speed())},
            {"walkableArea", toJson(input.walkableArea())}
        };
    }

    json toJson(const Planet &input)
    {
        auto planetSurfaceIds = json { };
        for (auto &planetSurface : input.planetSurfaces())
            planetSurfaceIds.push_back(planetSurface->definition.id);

        return json {
            {"type", input.type()},
            {"id", input.id},
            {"transform", toJson(input.transform())},
            {"planetSurfaces", planetSurfaceIds}
        };
    }

    json toJson(const PlacedItem &input)
    {
        return json {
            {"type", input.type()},
            {"id", input.id},
            {"transform", toJson(input.transform())},
            {"itemId", input.item->id}
        };
    }

    json toJson(const WalkableArea &input)
    {
        json charactersJson;
        for (auto character : input.characters())
            charactersJson.push_back(character->id);

        return json {
            {"characters", charactersJson},
            {"placedItems", toJsonArray(input.placedItems())}
        };
    }

    json toJson(const SpaceTransform &input)
    {
        return json {
            {"rotation", input.rotation},
            {"position", toJson(input.position)},
            {"scale", input.scale}
        };
    }
} // namespace space
