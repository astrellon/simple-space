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
#include "../../game/space_portal.hpp"
#include "../../game/planet.hpp"
#include "../../game/planet_surface.hpp"
#include "../../game/star_system.hpp"
#include "../../game/area.hpp"
#include "../../game/area_instances.hpp"
#include "../../game/items/placed_item.hpp"
#include "../../game/items/placeable_item.hpp"
#include "../../game/items/chair.hpp"
#include "../../game/items/teleporter.hpp"
#include "../../physics/polygon_collider.hpp"
#include "../../effects/grass_effect.hpp"

#include "../../controllers/character_controller.hpp"
#include "../../controllers/npc_controller.hpp"
#include "../../controllers/player_controller.hpp"
#include "../../controllers/space_station_controller.hpp"
#include "../../controllers/bird_controller.hpp"

#include "../loading_context.hpp"

#include "json_common.hpp"

namespace space
{
    json toJson(const GameSession &input)
    {
        auto result = json {
            {"spaceObjects", toJsonArray(input.spaceObjects())},
            {"items", toJsonArray(input.items())},
            {"playerController", toJson(input.playerController())},
            {"characterControllers", toJsonArray(input.characterControllers())}
        };

        return result;
    }

    std::unique_ptr<GameSession> fromJsonGameSession(Engine &engine, const json &j)
    {
        LoadingContext context;
        auto result = std::make_unique<GameSession>(engine);
        auto &session = *result.get();

        for (auto &item : j.at("items"))
            addFromJsonItem(item, session);

        for (auto &spaceObject : j.at("spaceObjects"))
            addFromJsonSpaceObject(spaceObject, session, context);

        for (auto &characterControllers : j.at("characterControllers"))
            addFromJsonCharacterController(characterControllers, session);

        addFromJsonPlayerController(j.at("playerController"), *result);

        result->onPostLoad(context);

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
        if (input.isGenerated())
            return json {};

        if (input.type() == Character::SpaceObjectType())
            return toJson(dynamic_cast<const Character &>(input));

        else if (input.type() == Ship::SpaceObjectType())
            return toJson(dynamic_cast<const Ship &>(input));

        else if (input.type() == Planet::SpaceObjectType())
            return toJson(dynamic_cast<const Planet &>(input));

        else if (input.type() == SpacePortal::SpaceObjectType())
            return toJson(dynamic_cast<const SpacePortal &>(input));

        else if (input.type() == PlacedItem::SpaceObjectType())
            return toJson(dynamic_cast<const PlacedItem &>(input));

        else if (input.type() == GrassEffect::SpaceObjectType())
            return toJson(dynamic_cast<const GrassEffect &>(input));

        throw std::runtime_error("Unknown space object type");
    }
    bool addFromJsonSpaceObject(const json &j, GameSession &session, LoadingContext &context)
    {
        auto type = j.at("type").get<std::string>();
        if (type == Character::SpaceObjectType())
            return addFromJsonCharacter(j, session);

        if (type == Ship::SpaceObjectType())
            return addFromJsonShip(j, session, context);

        if (type == Planet::SpaceObjectType())
            // Ignore planets
            return true;

        if (type == SpacePortal::SpaceObjectType())
            return addFromJsonSpacePortal(j, session);

        if (type == PlacedItem::SpaceObjectType())
            return addFromJsonPlacedItem(j, session);

        if (type == StarSystem::SpaceObjectType())
            return addFromJsonStarSystem(j, session, context);

        if (type == PlanetSurface::SpaceObjectType())
            return addFromJsonPlanetSurface(j, session, context);

        if (type == GrassEffect::SpaceObjectType())
            return addFromJsonGrassEffect(j, session);

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
        result["area"] = toJson(input.area());
        result["definitionId"] = input.definition.id;
        result["prevPosition"] = toJson(input.prevPosition());

        return result;
    }
    bool addFromJsonShip(const json &j, GameSession &session, LoadingContext &context)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const ShipDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find ship definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto result = session.createObject<Ship>(id, *definition);
        result->transform(fromJsonTransform(j.at("transform")));
        sf::Vector2f prevPosition;
        if (!Utils::json_try_get(j, "prevPosition", prevPosition))
        {
            prevPosition = result->transform().position;
        }
        result->prevPosition(prevPosition);

        result->rotationSpeed(j.at("rotationSpeed").get<float>());
        result->speed(fromJsonVector2f(j.at("speed")));

        auto areaJson = j.find("area");
        if (areaJson != j.end())
        {
            auto area = &result->area();
            auto instances = context.getAreaInstance(area);
            addFromJsonAreaInstances(*areaJson, instances);
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

    bool addFromJsonPlacedItem(const json &j, GameSession &session)
    {
        auto itemId = j.at("itemId").get<ItemId>();
        auto position = fromJsonVector2f(j.at("position"));

        PlaceableItem *item;
        if (!session.tryGetItem<PlaceableItem>(itemId, &item))
        {
            return false;
        }

        auto result = session.createObject<PlacedItem>(item);
        result->transform().position = position;

        return true;
    }

    json toJson(const SpacePortal &input)
    {
        auto result = toJsonBase(input);
        result["definitionId"] = input.definition.id;
        return result;
    }
    bool addFromJsonSpacePortal(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const SpacePortalDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find space portal definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto result = session.createObject<SpacePortal>(id, *definition);
        result->transform(fromJsonTransform(j.at("transform")));

        j.at("targetObjectId").get_to(result->targetObjectId);

        return true;
    }

    json toJson(const GrassEffect &input)
    {
        return json {
            {"type", input.type()},
            {"position", toJson(input.transform().position)},
            {"definitionId", input.definition.id},
            {"id", input.id}
        };
    }
    bool addFromJsonGrassEffect(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();
        auto position = fromJsonVector2f(j.at("position"));

        const GrassEffectDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find grass effect definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto result = session.createObject<GrassEffect>(id, *definition);
        result->transform().position = position;

        return true;
    }

    json toJson(const Area &input)
    {
        json objIdsJson;
        for (auto obj : input.objects())
            objIdsJson.push_back(obj->id);

        return json {
            {"objectIds", objIdsJson}
        };
    }

    bool addFromJsonAreaInstances(const json &j, AreaInstances *instances)
    {
        auto objectIds = j.at("objectIds");
        for (auto objectId : objectIds)
        {
            instances->addPostLoadObject(objectId);
        }

        return true;
    }

    json toJson(const StarSystem &input)
    {
        return json {
            {"definitionId", input.definition.id},
            {"area", toJson(input.area())},
            {"partOfLivePhoto", input.isPartOfLivePhoto()}
        };
    }
    bool addFromJsonStarSystem(const json &j, GameSession &session, LoadingContext &context)
    {
        auto definitionId = j.at("definitionId").get<DefinitionId>();
        const StarSystemDefinition *definition;
        if (!session.engine().definitionManager().tryGet<StarSystemDefinition>(definitionId, &definition))
        {
            std::cout << "Unable to find star system id: " << definitionId << std::endl;
            return false;
        }

        auto partOfLivePhoto = false;
        Utils::json_try_get(j, "partOfLivePhoto", partOfLivePhoto);

        auto starSystem = session.createObject<StarSystem>(session, definition->id, *definition, partOfLivePhoto);
        starSystem->init(session);
        auto instances = context.getAreaInstance(&starSystem->area());
        addFromJsonAreaInstances(j.at("area"), instances);

        return true;
    }

    json toJson(const PlanetSurface &input)
    {
        return json {
            {"planetId", input.partOfPlanet()->id},
            {"definitionId", input.definition.id},
            {"area", toJson(input.area())},
            {"partOfLivePhoto", input.isPartOfLivePhoto()}
        };
    }
    bool addFromJsonPlanetSurface(const json &j, GameSession &session, LoadingContext &context)
    {
        auto definitionId = j.at("definitionId").get<DefinitionId>();
        const PlanetSurfaceDefinition *definition;
        if (!session.engine().definitionManager().tryGet<PlanetSurfaceDefinition>(definitionId, &definition))
        {
            std::cout << "Unable to find planet surface id: " << definitionId << std::endl;
            return false;
        }

        auto planetId = j.at("planetId").get<ObjectId>();
        Planet *planet;
        if (!session.tryGetSpaceObject<Planet>(planetId, &planet))
        {
            std::cout << "Unable to find planet " << planetId << " for planet surface" << std::endl;
            return false;
        }

        auto partOfLivePhoto = false;
        Utils::json_try_get(j, "partOfLivePhoto", partOfLivePhoto);

        auto planetSurface = session.createObject<PlanetSurface>(definition->id, *definition, partOfLivePhoto);
        planetSurface->partOfPlanet(planet);

        auto instances = context.getAreaInstance(&planetSurface->area());
        addFromJsonAreaInstances(j.at("area"), instances);

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

        for (auto &itemId : itemIds)
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
        ControllingValue controlling = ControlNone;
        if (Utils::json_try_get<ControllingValue>(j, "controlling", controlling))
        {
            controller.controlling(controlling);
        }

        ObjectId controllingCharacterId, controllingShipId;

        if (Utils::json_try_get(j, "controllingCharacter", controllingCharacterId))
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

        if (Utils::json_try_get(j, "controllingShip", controllingShipId))
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

        auto inventoryJson = j.find("inventory");
        if (inventoryJson != j.end())
        {
            auto inventory = fromJsonInventory(*inventoryJson, session);
            controller.inventory(std::move(inventory));
        }

        return true;
    }

    json toJson(const CharacterController &input)
    {
        if (input.type() == NpcController::ControllerType())
            return toJson(dynamic_cast<const NpcController &>(input));

        else if (input.type() == PlayerController::ControllerType())
            return toJson(dynamic_cast<const PlayerController &>(input));

        else if (input.type() == SpaceStationController::ControllerType())
            return toJson(dynamic_cast<const SpaceStationController &>(input));

        else if (input.type() == BirdController::ControllerType())
            return toJson(dynamic_cast<const BirdController &>(input));

        throw std::runtime_error("Unknown character controller");
    }

    bool addFromJsonCharacterController(const json &j, GameSession &session)
    {
        auto type = j.at("type").get<std::string>();
        if (type == NpcController::ControllerType())
            return addFromJsonNpcController(j, session);

        if (type == SpaceStationController::ControllerType())
            return addFromJsonSpaceStationController(j, session);

        if (type == BirdController::ControllerType())
            return addFromJsonBirdController(j, session);

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
        auto controller = session.createCharacterController<NpcController>();
        if (!addFromJsonCharacterControllerBase(j, session, *controller))
        {
            return false;
        }

        auto dialogueId = j.find("dialogue");
        if (dialogueId != j.end())
        {
            auto id = dialogueId->get<std::string>();
            const Dialogue *dialogue;
            if (session.engine().definitionManager().tryGet<Dialogue>(id, &dialogue))
            {
                controller->dialogue(dialogue);
            }
            else
            {
                std::cout << "Unable to find dialogue '" << id << "' for NPC" << std::endl;
            }
        }

        return true;
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

    json toJson(const SpaceStationController &input)
    {
        return toJsonBase(input);
    }

    bool addFromJsonSpaceStationController(const json &j, GameSession &session)
    {
        auto controller = session.createCharacterController<SpaceStationController>();
        if (!addFromJsonCharacterControllerBase(j, session, *controller))
        {
            return false;
        }

        return true;
    }

    json toJson(const BirdController &input)
    {
        return toJsonBase(input);
    }

    bool addFromJsonBirdController(const json &j, GameSession &session)
    {
        auto controller = session.createCharacterController<BirdController>();
        if (!addFromJsonCharacterControllerBase(j, session, *controller))
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
            return toJson(dynamic_cast<const PlaceableItem &>(item));
        if (item.type() == Chair::ItemType())
            return toJson(dynamic_cast<const Chair &>(item));
        if (item.type() == Teleporter::ItemType())
            return toJson(dynamic_cast<const Teleporter &>(item));

        throw std::runtime_error("Unknown item type");
    }

    bool addFromJsonItem(const json &j, GameSession &session)
    {
        auto type = j.at("type").get<std::string>();
        if (type == PlaceableItem::ItemType())
            return addFromJsonPlaceableItem(j, session);
        if (type == Chair::ItemType())
            return addFromJsonChair(j, session);
        if (type == Teleporter::ItemType())
            return addFromJsonTeleporter(j, session);

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

    json toJson(const Chair &item)
    {
        return toJsonBase(item);
    }

    bool addFromJsonChair(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ItemId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const PlaceableItemDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find placeable item definition " << definitionId << " for chair " << id << std::endl;
            return false;
        }

        session.createItem<Chair>(id, *definition);

        return true;
    }

    json toJson(const Teleporter &item)
    {
        return toJsonBase(item);
    }

    bool addFromJsonTeleporter(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ItemId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const PlaceableItemDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find placeable item definition " << definitionId << " for teleporter " << id << std::endl;
            return false;
        }

        session.createItem<Teleporter>(id, *definition);

        return true;
    }
} // namespace space
