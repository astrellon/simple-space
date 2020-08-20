#include "json_serialisers_game.hpp"

#include "../../game_session.hpp"
#include "../../engine.hpp"
#include "../../space_transform.hpp"

#include "../../definitions/dialogue.hpp"
#include "../../definition_manager.hpp"
#include "../../game/space_object.hpp"
#include "../../game/character.hpp"
#include "../../game/ship.hpp"
#include "../../game/planet.hpp"
#include "../../game/planet_surface.hpp"
#include "../../game/star_system.hpp"
#include "../../game/walkable_area.hpp"
#include "../../game/items/placed_item.hpp"
#include "../../game/items/placeable_item.hpp"
#include "../../physics/polygon_collider.hpp"

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
            {"npcControllers", toJsonArray(input.npcControllers())},
            {"starSystems", toJsonArray(input.starSystems())},
            {"planetSurfaces", toJsonArray(input.planetSurfaces())}
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
    std::unique_ptr<SpaceObject> fromJsonSpaceObject(const json &j, GameSession &session)
    {
        auto type = j.at("type").get<std::string>();
        if (type == Character::SpaceObjectType())
            return fromJsonCharacter(j, session);

        throw std::runtime_error("Unknown space object type");
    }

    json toJson(const Character &input)
    {
        auto result = toJsonBase(input);
        result["definitionId"] = input.definition.id;
        return result;
    }
    std::unique_ptr<Character> fromJsonCharacter(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<std::string>();
        auto definitionId = j.at("definitionId").get<std::string>();

        const CharacterDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            throw std::runtime_error("Unable to find definition for character");
        }

        auto result = std::make_unique<Character>(id, *definition);
        result->transform(fromJsonTransform(j.at("transform")));

        return result;
    }

    json toJson(const Ship &input)
    {
        auto result = toJsonBase(input);
        result["rotationSpeed"] = input.rotationSpeed();
        result["speed"] = toJson(input.speed());
        result["walkableArea"] = toJson(input.walkableArea());
        result["definitionId"] = input.definition.id;

        return result;
    }
    std::unique_ptr<Ship> fromJsonShip(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<std::string>();
        auto definitionId = j.at("definitionId").get<std::string>();

        const ShipDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            throw std::runtime_error("Unable to find definition for ship");
        }

        auto walkableArea = fromJsonWalkableArea(j.at("walkableArea"), session);

        auto result = std::make_unique<Ship>(id, *definition, std::move(walkableArea));
        result->transform(fromJsonTransform(j.at("transform")));

        result->rotationSpeed(j.at("rotationSpeed").get<float>());
        result->speed(fromJsonVector2f(j.at("speed")));

        return result;
    }

    json toJson(const Planet &input)
    {
        auto result = toJsonBase(input);

        auto planetSurfaceIds = json { };
        for (auto &planetSurface : input.planetSurfaces())
            planetSurfaceIds.push_back(planetSurface->definition.id);

        result["planetSurfaceIds"] = planetSurfaceIds;
        result["definitionId"] = input.definition.id;

        return result;
    }
    std::unique_ptr<Planet> fromJsonPlanet(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<std::string>();
        auto definitionId = j.at("definitionId").get<std::string>();

        const PlanetDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            throw std::runtime_error("Unable to find definition for planet");
        }

        auto result = std::make_unique<Planet>(id, *definition);

        auto planetSurfaceIds = j.at("planetSurfaceIds");
        for (auto &iter : planetSurfaceIds)
        {
            auto id = iter.get<std::string>();
            PlanetSurface *planetSurface;
            if (!session.tryGetPlanetSurface(id, &planetSurface))
            {
                std::cout << "ERROR Could not find planet surface: " << id << std::endl;
                continue;
            }

            result->addPlanetSurface(planetSurface);
        }

        return result;
    }

    json toJson(const PlacedItem &input)
    {
        return json {
            {"type", input.type()},
            {"position", toJson(input.transform().position)},
            {"itemId", input.item->id}
        };
    }

    bool addFromJsonPlacedItem(const json &j, GameSession &session, WalkableArea &area)
    {
        auto itemId = j.at("itemId").get<ItemId>();
        PlaceableItem *item;
        if (!session.tryGetItem<PlaceableItem>(itemId, &item))
        {
            return false;
        }

        auto position = fromJsonVector2f(j.at("position"));
        area.addPlaceable(item, position);

        return true;
    }

    json toJson(const WalkableArea &input)
    {
        json charactersJson;
        for (auto character : input.characters())
            charactersJson.push_back(character->id);

        return json {
            {"characterIds", charactersJson},
            {"placedItems", toJsonArray(input.placedItems())}
        };
    }

    std::unique_ptr<WalkableArea> fromJsonWalkableArea(const json &j, GameSession &session)
    {
        auto result = std::make_unique<WalkableArea>();
        auto characterIds = j.at("characterIds");
        for (auto jsonId : characterIds)
        {
            auto id = jsonId.get<std::string>();
            Character *character;
            if (!session.tryGetSpaceObject<Character>(id, &character))
            {
                std::cout << "Unable to find character '" << id << "' for walkable area" << std::endl;
                continue;
            }

            result->addCharacter(character);
        }

        auto placedItems = j.at("placedItems");
        for (auto placedItem : placedItems)
        {
            addFromJsonPlacedItem(placedItem, session, *result.get());
        }

        return result;
    }

    json toJson(const StarSystem &input)
    {
        json spaceObjectIds;
        for (auto obj : input.objects())
            spaceObjectIds.push_back(obj->id);

        return json {
            {"definitionId", input.definition.id},
            {"spaceObjectIds", }
        };
    }

    json toJson(const PlanetSurface &input)
    {
        return json {
            {"definitionId", input.definition.id},
            {"walkableArea", toJson(input.walkableArea())}
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

} // namespace space
