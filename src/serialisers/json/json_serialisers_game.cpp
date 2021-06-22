#include "json_serialisers_game.hpp"

#include "../../game_session.hpp"
#include "../../engine.hpp"
#include "../../space_transform.hpp"
#include "../../utils.hpp"
#include "../../photo_album.hpp"

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
#include "../../game/items/food_item.hpp"
#include "../../game/items/bed_item.hpp"
#include "../../game/live_photo.hpp"
#include "../../physics/polygon_collider.hpp"
#include "../../effects/grass_effect.hpp"

#include "../../controllers/character_controller.hpp"
#include "../../controllers/npc_controller.hpp"
#include "../../controllers/player_controller.hpp"
#include "../../controllers/space_station_controller.hpp"
#include "../../controllers/bird_controller.hpp"

#include "../loading_context.hpp"

#include "json_common.hpp"

#define SpaceObjectToJson(Type) { \
    const Type *casted; \
    if (input.tryCast(casted)) return toJson(*casted); \
}

#define SpaceObjectFromJson(Type) { \
    if (objType == Type::TypeValue) \
        return addFromJson##Type(j, session, context); \
}

#define ItemToJson(Type) { \
    const Type *casted; \
    if (input.tryCast(casted)) return toJson(*casted); \
}

#define ItemFromJson(Type) { \
    if (itemType == Type::TypeValue) \
        return addFromJson##Type(j, session); \
}

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

        return result;
    }

    json toJsonBase(const SpaceObject &input)
    {
        json result {
            {"type", toString(input.type2)},
            {"id", input.id},
            {"transform", toJson(input.transform())}
        };

        return result;
    }

    json toJson(const SpaceObject &input)
    {
        if (input.isGenerated())
            return json {};

        SpaceObjectToJson(Character);
        SpaceObjectToJson(Ship);
        SpaceObjectToJson(Planet);
        SpaceObjectToJson(SpacePortal);
        SpaceObjectToJson(PlacedItem);
        SpaceObjectToJson(GrassEffect);
        SpaceObjectToJson(StarSystem);
        SpaceObjectToJson(PlanetSurface);
        SpaceObjectToJson(LivePhoto);
        SpaceObjectToJson(LivePhotoTarget);

        throw std::runtime_error("Unknown space object type");
    }
    bool addFromJsonSpaceObject(const json &j, GameSession &session, LoadingContext &context)
    {
        auto type = j.at("type").get<std::string>();
        auto objType = fromStringSpaceObjectType(type);

        if (objType == Planet::TypeValue)
        {
            return true;
        }

        SpaceObjectFromJson(Character);
        SpaceObjectFromJson(Ship);
        SpaceObjectFromJson(SpacePortal);
        SpaceObjectFromJson(PlacedItem);
        SpaceObjectFromJson(StarSystem);
        SpaceObjectFromJson(PlanetSurface);
        SpaceObjectFromJson(GrassEffect);
        SpaceObjectFromJson(LivePhoto);
        SpaceObjectFromJson(LivePhotoTarget);

        throw std::runtime_error("Unknown space object type");
    }
    void applyBaseFromJson(const json &j, SpaceObject &input, LoadingContext &context)
    {
        input.transform(fromJsonTransform(j.at("transform")));
    }

    json toJson(const Character &input)
    {
        auto result = toJsonBase(input);
        result["definitionId"] = input.definition.id;
        result["sprite"] = toJson(input.sprite());
        return result;
    }
    bool addFromJsonCharacter(const json &j, GameSession &session, LoadingContext &context)
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
        applyBaseFromJson(j, *result, context);

        auto findSprite = j.find("sprite");
        if (findSprite != j.end())
        {
            applyFromJsonAnimatedSprite(*findSprite, result->sprite());
        }

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
        applyBaseFromJson(j, *result, context);

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
        json result {
            {"id", input.id},
            {"type", toString(input.type2)},
            {"position", toJson(input.transform().position)},
            {"itemId", input.item->id}
        };

        return result;
    }

    bool addFromJsonPlacedItem(const json &j, GameSession &session, LoadingContext &context)
    {
        ObjectId id;
        auto hasId = Utils::json_try_get(j, "id", id);

        auto itemId = j.at("itemId").get<ItemId>();
        auto position = fromJsonVector2f(j.at("position"));

        PlaceableItem *item;
        if (!session.tryGetItem<PlaceableItem>(itemId, &item))
        {
            return false;
        }

        PlacedItem *result = nullptr;
        if (hasId)
        {
            result = session.createObject<PlacedItem>(id, item);
        }
        else
        {
            result = session.createObject<PlacedItem>(item);
        }
        result->transform().position = position;

        return true;
    }

    json toJson(const SpacePortal &input)
    {
        auto result = toJsonBase(input);
        result["definitionId"] = input.definition.id;
        return result;
    }
    bool addFromJsonSpacePortal(const json &j, GameSession &session, LoadingContext &context)
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
        applyBaseFromJson(j, *result, context);

        j.at("targetObjectId").get_to(result->targetObjectId);

        return true;
    }

    json toJson(const GrassEffect &input)
    {
        auto result = toJsonBase(input);
        result["definitionId"] = input.definition.id;
        return result;
    }
    bool addFromJsonGrassEffect(const json &j, GameSession &session, LoadingContext &context)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const GrassEffectDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find grass effect definition " << definitionId << " for " << id << std::endl;
            return false;
        }

        auto result = session.createObject<GrassEffect>(id, *definition);
        applyBaseFromJson(j, *result, context);

        return true;
    }

    json toJson(const LivePhoto &input)
    {
        auto result = toJsonBase(input);
        result["targetId"] = input.targetObject()->id;
        result["photoSize"] = toJson(input.photoSize);
        return result;
    }
    bool addFromJsonLivePhoto(const json &j, GameSession &session, LoadingContext &context)
    {
        auto id = j.at("id").get<ObjectId>();

        sf::Vector2u photoSize(256, 256);
        Utils::json_try_get(j, "photoSize", photoSize);

        auto result = session.createObject<LivePhoto>(id, photoSize);
        applyBaseFromJson(j, *result, context);

        ObjectId livePhotoTargetId;
        if (Utils::json_try_get(j, "targetId", livePhotoTargetId))
        {
            context.postLoadObjectIds[id][LoadingType::LivePhotoTarget] = livePhotoTargetId;
        }

        result->init(session.engine());

        return true;
    }

    json toJson(const LivePhotoTarget &input)
    {
        return toJsonBase(input);
    }
    bool addFromJsonLivePhotoTarget(const json &j, GameSession &session, LoadingContext &context)
    {
        auto id = j.at("id").get<ObjectId>();

        auto result = session.createObject<LivePhotoTarget>(id);
        applyBaseFromJson(j, *result, context);

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
        json result {
            {"id", input.id},
            {"definitionId", input.definition.id},
            {"area", toJson(input.area())}
        };

        return result;
    }
    bool addFromJsonStarSystem(const json &j, GameSession &session, LoadingContext &context)
    {
        auto id = j.at("id").get<ObjectId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();
        const StarSystemDefinition *definition;
        if (!session.engine().definitionManager().tryGet<StarSystemDefinition>(definitionId, &definition))
        {
            std::cout << "Unable to find star system id: " << definitionId << std::endl;
            return false;
        }

        auto starSystem = session.createObject<StarSystem>(session, id, *definition);
        starSystem->init(session);
        auto instances = context.getAreaInstance(&starSystem->area());
        addFromJsonAreaInstances(j.at("area"), instances);

        return true;
    }

    json toJson(const PlanetSurface &input)
    {
        json result {
            {"id", input.id},
            {"planetId", input.partOfPlanet()->id},
            {"definitionId", input.definition.id},
            {"area", toJson(input.area())},
            {"type", toString(input.type2)}
        };

        return result;
    }
    bool addFromJsonPlanetSurface(const json &j, GameSession &session, LoadingContext &context)
    {
        auto id = j.at("id").get<ObjectId>();
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

        auto planetSurface = session.createObject<PlanetSurface>(id, *definition);
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
            {"type", toString(item.type2)}
        };
    }

    json toJson(const Item &input)
    {
        ItemToJson(PlaceableItem);
        ItemToJson(Chair);
        ItemToJson(Teleporter);
        ItemToJson(FoodItem);
        ItemToJson(BedItem);

        throw std::runtime_error("Unknown item type");
    }

    bool addFromJsonItem(const json &j, GameSession &session)
    {
        auto type = j.at("type").get<std::string>();
        auto itemType = fromStringItemType(type);

        ItemFromJson(PlaceableItem);
        ItemFromJson(Chair);
        ItemFromJson(Teleporter);
        ItemFromJson(FoodItem);
        ItemFromJson(BedItem);

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

    json toJson(const FoodItem &item)
    {
        return toJsonBase(item);
    }

    bool addFromJsonFoodItem(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ItemId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const PlaceableItemDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find placeable item definition " << definitionId << " for food item " << id << std::endl;
            return false;
        }

        session.createItem<FoodItem>(id, *definition);

        return true;
    }

    json toJson(const BedItem &item)
    {
        return toJsonBase(item);
    }

    bool addFromJsonBedItem(const json &j, GameSession &session)
    {
        auto id = j.at("id").get<ItemId>();
        auto definitionId = j.at("definitionId").get<DefinitionId>();

        const PlaceableItemDefinition *definition;
        if (!session.engine().definitionManager().tryGet(definitionId, &definition))
        {
            std::cout << "Unable to find placeable item definition " << definitionId << " for bed item " << id << std::endl;
            return false;
        }

        session.createItem<BedItem>(id, *definition);

        return true;
    }

    json toJson(const PhotoAlbum &input)
    {
        json photoIdsJson;
        for (auto photo : input.photos())
            photoIdsJson.push_back(photo->id);

        return json {
            {"photoIds", photoIdsJson}
        };
    }
    bool addFromJsonPhotoAlbum(const json &j, GameSession &session)
    {
        auto photoIds = j.at("photoIds");
        for (auto photoId : photoIds)
        {
            //instances->addPostLoadObject(objectId);
        }

        return true;
    }

    json toJson(const AnimatedSprite &sprite)
    {
        return json {
            {"animatedTime", sprite.animationTime},
            {"sequence", sprite.currentAnimation()}
        };
    }
    void applyFromJsonAnimatedSprite(const json &j, AnimatedSprite &sprite)
    {
        std::string sequence;
        if (Utils::json_try_get(j, "sequence", sequence))
        {
            sprite.sequence(sequence, true);
        }

        float animatedTime = 0;
        if (Utils::json_try_get(j, "animatedTime", animatedTime))
        {
            sprite.animationTime = animatedTime;
        }

    }
} // namespace space
