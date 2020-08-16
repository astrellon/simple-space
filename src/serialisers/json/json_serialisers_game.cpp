#include "json_serialisers_game.hpp"

#include "../../game_session.hpp"
#include "../../space_transform.hpp"

#include "../../definitions/dialogue.hpp"

#include "../../game/space_object.hpp"
#include "../../game/character.hpp"
#include "../../game/ship.hpp"
#include "../../game/planet.hpp"
#include "../../game/planet_surface.hpp"
#include "../../game/star_system.hpp"
#include "../../game/walkable_area.hpp"
#include "../../game/items/placed_item.hpp"
#include "../../game/items/placeable_item.hpp"

#include "../../controllers/character_controller.hpp"
#include "../../controllers/npc_controller.hpp"
#include "../../controllers/player_controller.hpp"

#include "json_common.hpp"

namespace space
{
    json toJson(const GameSession &input)
    {
        auto result = json {
            {"spaceObjects", toJsonArray(input.spaceObjects())},
            {"items", toJsonArray(input.items())},
            {"playerController", toJson(input.playerController())},
            {"npcControllers", toJsonArray(input.npcControllers())}
        };

        if (input.activeStarSystem())
            result["activeStarSystem"] = input.activeStarSystem()->definition.id;

        if (input.activePlanetSurface())
            result["activePlanetSurface"] = input.activePlanetSurface()->definition.id;

        return result;
    }

    std::unique_ptr<GameSession> fromJsonGameSession(Engine &engine, const json &j)
    {
        return std::make_unique<GameSession>(engine);
    }

    json toJsonBase(const SpaceObject &input)
    {
        return json {
            {"type", input.type()},
            {"id", input.id},
            {"transform", toJson(input.transform())},
        };
    }
    json toJson(const SpaceObject &input)
    {
        if (input.type() == Character::SpaceObjectType())
            return toJson(dynamic_cast<const Character &>(input));

        else if (input.type() == Ship::SpaceObjectType())
            return toJson(dynamic_cast<const Ship &>(input));

        else if (input.type() == Planet::SpaceObjectType())
            return toJson(dynamic_cast<const Planet &>(input));

        else if (input.type() == PlacedItem::SpaceObjectType())
            return toJson(dynamic_cast<const PlacedItem &>(input));

        throw std::runtime_error("Unknown space object type");
    }

    json toJson(const Character &input)
    {
        return toJsonBase(input);
    }

    json toJson(const Ship &input)
    {
        auto result = toJsonBase(input);
        result["rotationSpeed"] = input.rotationSpeed();
        result["speed"] = toJson(input.speed());
        result["walkableArea"] = toJson(input.walkableArea());

        return result;
    }

    json toJson(const Planet &input)
    {
        auto result = toJsonBase(input);

        auto planetSurfaceIds = json { };
        for (auto &planetSurface : input.planetSurfaces())
            planetSurfaceIds.push_back(planetSurface->definition.id);

        result["planetSurfaces"] = planetSurfaceIds;

        return result;
    }

    json toJson(const PlacedItem &input)
    {
        auto result = toJsonBase(input);
        result["itemId"] = input.item->id;
        return result;
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

    json toJson(const Inventory &input)
    {
        json result;
        for (auto item : input.items())
            result.push_back(item->id);

        return result;
    }

    json toJsonBase(const CharacterController &input)
    {
        auto result = json {
            {"inventory", toJson(input.inventory())},
            {"controlling", input.controlling()}
        };

        if (input.controllingCharacter())
            result["controllingCharacter"] = input.controllingCharacter()->id;

        if (input.controllingShip())
            result["controllingShip"] = input.controllingShip()->id;

        return result;
    }

    json toJson(const CharacterController &input)
    {
        if (input.type() == NpcController::ControllerType())
            return toJson(dynamic_cast<const NpcController &>(input));

        else if (input.type() == PlayerController::ControllerType())
            return toJson(dynamic_cast<const PlayerController &>(input));

        throw std::runtime_error("Unknown character controller");
    }

    json toJson(const NpcController &input)
    {
        auto result = toJsonBase(input);
        if (input.dialogue())
            result["dialogue"] = input.dialogue()->id;

        return result;
    }

    json toJson(const PlayerController &input)
    {
        return toJsonBase(input);
    }

    json toJsonBase(const Item &item)
    {
        return json {
            {"id", item.id},
            {"definitionId", item.definition.id},
            {"type", item.type()}
        };
    }

    json toJson(const Item &item)
    {
        if (item.type() == PlaceableItem::ItemType())
        {
            return toJson(dynamic_cast<const PlaceableItem &>(item));
        }

        throw std::runtime_error("Unknown item type");
    }

    json toJson(const PlaceableItem &item)
    {
        return toJsonBase(item);
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
