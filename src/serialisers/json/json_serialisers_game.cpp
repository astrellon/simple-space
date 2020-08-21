#include "json_serialisers_game.hpp"

#include "../../game_session.hpp"
#include "../../engine.hpp"
#include "../../space_transform.hpp"
#include "../../utils.hpp"

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
        auto result = std::make_unique<GameSession>(engine);
        auto &session = *result.get();

        for (auto spaceObject : j.at("spaceObjects"))
            addFromJsonSpaceObject(j, session);

        for (auto item : j.at("items"))
            addFromJsonItem(j, session);

        for (auto npcControllers : j.at("npcControllers"))
            addFromJsonNpcController(j, session);

        for (auto starSystem : j.at("starSystems"))
            addFromJsonStarSystem(j, session);

        for (auto planetSurfaces : j.at("planetSurfaces"))
            addFromJsonPlanetSurface(j, session);

        auto activeStarSystem = j.find("activeStarSystem");
        if (activeStarSystem != j.end())
        {
            auto activeStarSystemId = activeStarSystem->get<DefinitionId>();
            StarSystem *starSystem;
            if (!session.tryGetStarSystem(activeStarSystemId, &starSystem))
            {
                session.activeStarSystem(starSystem);
            }
            else
            {
                std::cout << "Unable to find active star system " << activeStarSystemId << std::endl;
            }
        }

        auto activePlanetSurface = j.find("activePlanetSurface");
        if (activePlanetSurface != j.end())
        {
            auto activePlanetSurfaceId = activePlanetSurface->get<DefinitionId>();
            PlanetSurface *planetSurface;
            if (!session.tryGetPlanetSurface(activePlanetSurfaceId, &planetSurface))
            {
                session.activePlanetSurface(planetSurface);
            }
            else
            {
                std::cout << "Unable to find active planet surface " << activePlanetSurfaceId << std::endl;
            }
        }

        return std::move(result);
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
    bool addFromJsonSpaceObject(const json &j, GameSession &session)
    {
        auto type = j.at("type").get<std::string>();
        if (type == Character::SpaceObjectType())
            return addFromJsonCharacter(j, session);

        if (type == Ship::SpaceObjectType())
            return addFromJsonShip(j, session);

        if (type == Planet::SpaceObjectType())
            return addFromJsonPlanet(j, session);

        if (type == PlacedItem::SpaceObjectType())
            throw std::runtime_error("Unable to add placed item without walkable area");

        throw std::runtime_error("Unknown space object type");
    }

    json toJson(const Character &input)
    {
        auto result = toJsonBase(input);
        result["definitionId"] = input.definition.id;
        return result;
    }
    bool addFromJsonCharacter(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const CharacterDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find character definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto result = session.createObject<Character>(id, *definition);
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
    bool addFromJsonShip(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const ShipDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find ship definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto walkableArea = fromJsonWalkableArea(j.at("walkableArea"), session);

        auto result = session.createObject<Ship>(id, *definition, std::move(walkableArea));
        result->transform(fromJsonTransform(j.at("transform")));

        result->rotationSpeed(j.at("rotationSpeed").get<float>());
        result->speed(fromJsonVector2f(j.at("speed")));

        return true;
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
    bool addFromJsonPlanet(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const PlanetDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find planet definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto result = session.createObject<Planet>(id, *definition);

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

        return true;
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
            std::cout << "Unable to find placeable item for placed item: " << itemId << std::endl;
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
    bool addFromJsonStarSystem(const json &j, GameSession &session)
    {
        auto definitionId = j.at("definitionId").get<DefinitionId>();
        const StarSystemDefinition *definition;
        if (!session.engine().definitionManager().tryGet<StarSystemDefinition>(definitionId, &definition))
        {
            std::cout << "Unable to find star system id: " << definitionId << std::endl;
            return false;
        }

        auto starSystem = session.createStarSystem(*definition);

        auto spaceObjectIds = j.at("spaceObjectIds");
        for (auto spaceObjectId : spaceObjectIds)
        {
            auto id = spaceObjectId.get<ObjectId>();
            SpaceObject *obj;
            if (!session.tryGetSpaceObject(id, &obj))
            {
                std::cout << "Unable to find space object " << id << " for star system " << definitionId << std::endl;
                continue;
            }

            starSystem->addObject(obj);
        }

        return true;
    }

    json toJson(const PlanetSurface &input)
    {
        return json {
            {"definitionId", input.definition.id},
            {"walkableArea", toJson(input.walkableArea())}
        };
    }
    bool addFromJsonPlanetSurface(const json &j, GameSession &session)
    {
        auto definitionId = j.at("definitionId").get<DefinitionId>();
        const PlanetSurfaceDefinition *definition;
        if (!session.engine().definitionManager().tryGet<PlanetSurfaceDefinition>(definitionId, &definition))
        {
            std::cout << "Unable to find planet surface id: " << definitionId << std::endl;
            return false;
        }

        auto walkableArea = fromJsonWalkableArea(j.at("walkableArea"), session);
        auto planetSurface = session.createPlanetSurface(*definition, std::move(walkableArea));

        return true;
    }

    json toJson(const Inventory &input)
    {
        json itemIds;
        for (auto item : input.items())
            itemIds.push_back(item->id);

        return json {
            {"items", itemIds}
        };
    }

    std::unique_ptr<Inventory> fromJsonInventory(const json &j, GameSession &session)
    {
        auto itemIds = j.at("items");
        auto result = std::make_unique<Inventory>();

        for (auto itemId : itemIds)
        {
            auto id = itemId.get<ItemId>();
            Item *item;
            if (!session.tryGetItem(id, &item))
            {
                std::cout << "Unable to find item for inventory: " << id << std::endl;
                continue;
            }

            result->addItem(item);
        }

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

    bool addFromJsonCharacterControllerBase(const json &j, GameSession &session, CharacterController &controller)
    {
        auto controlling = j.at("controlling").get<std::string>();
        ObjectId controllingCharacterId, controllingShipId;

        if (Utils::json_try_set(j, "controllingCharacter", controllingCharacterId))
        {
            Character *character;
            if (session.tryGetSpaceObject<Character>(controllingCharacterId, &character))
            {
                controller.controllingCharacter(character);
            }
            else
            {
                std::cout << "Unable to find character for controller: " << controllingCharacterId << std::endl;
            }
        }

        if (Utils::json_try_set(j, "controllingShip", controllingShipId))
        {
            Ship *ship;
            if (session.tryGetSpaceObject<Ship>(controllingShipId, &ship))
            {
                controller.controllingShip(ship);
            }
            else
            {
                std::cout << "Unable to find ship for controller: " << controllingShipId << std::endl;
            }
        }

        auto inventory = fromJsonInventory(j.at("inventory"), session);
        controller.inventory(std::move(inventory));

        return true;
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

    bool addFromJsonNpcController(const json &j, GameSession &session)
    {
        auto controller = session.createNpcController();
        return addFromJsonCharacterControllerBase(j, session, *controller);
    }

    json toJson(const PlayerController &input)
    {
        return toJsonBase(input);
    }

    bool addFromJsonPlayerController(const json &j, GameSession &session)
    {
        if (!addFromJsonCharacterControllerBase(j, session, session.playerController()))
        {
            return false;
        }

        return true;
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

    bool addFromJsonItem(const json &j, GameSession &session)
    {
        auto type = j.at("type").get<std::string>();
        if (type == PlaceableItem::ItemType())
            return addFromJsonPlaceableItem(j, session);

        throw std::runtime_error("Unknown item type");
    }

    json toJson(const PlaceableItem &item)
    {
        return toJsonBase(item);
    }

    bool addFromJsonPlaceableItem(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ItemId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const PlaceableItemDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find placeable item definition " << definitionId << " for item " << id << std::endl;
            return false;
        }

        session.createItem<PlaceableItem>(id, *definition);

        return true;
    }
} // namespace space
