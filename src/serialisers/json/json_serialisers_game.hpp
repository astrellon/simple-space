#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "json.hpp"

using nlohmann::json;

namespace space
{
    class GameSession;
    class Engine;
    class SpaceObject;
    class Character;
    class Ship;
    class Planet;
    class PlacedItem;
    class Area;
    class AreaInstances;
    class SpaceTransform;
    class CharacterController;
    class NpcController;
    class PlayerController;
    class SpaceStationController;
    class BirdController;
    class Inventory;
    class Item;
    class PlaceableItem;
    class DefinitionManager;
    class StarSystem;
    class PlanetSurface;
    class Chair;
    class Teleporter;
    class SpacePortal;
    class GrassEffect;
    class LoadingContext;

    json toJson(const GameSession &input);
    std::unique_ptr<GameSession> fromJsonGameSession(Engine &engine, const json &j);

    json toJsonBase(const SpaceObject &input);
    json toJson(const SpaceObject &input);
    bool addFromJsonSpaceObject(const json &j, GameSession &session, LoadingContext &context);

    json toJson(const Character &input);
    bool addFromJsonCharacter(const json &j, GameSession &session);

    json toJson(const Ship &input);
    bool addFromJsonShip(const json &j, GameSession &session, LoadingContext &context);

    json toJson(const PlacedItem &input);
    bool addFromJsonPlacedItem(const json &j, AreaInstances &instances);

    bool addFromJsonGrassEffect(const json &j, AreaInstances &instances);

    json toJson(const Area &input);
    bool addFromJsonAreaInstances(const json &j, AreaInstances *instances);

    json toJson(const StarSystem &input);
    bool addFromJsonStarSystem(const json &j, GameSession &session, LoadingContext &context);

    json toJson(const PlanetSurface &input);
    bool addFromJsonPlanetSurface(const json &j, GameSession &session, LoadingContext &context);

    json toJson(const Inventory &input);
    std::unique_ptr<Inventory> fromJsonInventory(const json &j, GameSession &session);

    json toJsonBase(const CharacterController &input);
    bool addFromJsonCharacterControllerBase(const json &j, GameSession &session, CharacterController &controller);

    json toJson(const CharacterController &input);
    bool addFromJsonCharacterController(const json &j, GameSession &session);

    json toJson(const NpcController &input);
    bool addFromJsonNpcController(const json &j, GameSession &session);

    json toJson(const PlayerController &input);
    bool addFromJsonPlayerController(const json &j, GameSession &session);

    json toJson(const SpaceStationController &input);
    bool addFromJsonSpaceStationController(const json &j, GameSession &session);

    json toJson(const BirdController &input);
    bool addFromJsonBirdController(const json &j, GameSession &session);

    json toJson(const SpacePortal &input);
    bool addFromJsonSpacePortal(const json &j, GameSession &session);

    json toJson(const GrassEffect &input);
    bool addFromJsonGrassEffect(const json &j, GameSession &session);

    json toJsonBase(const Item &item);
    json toJson(const Item &item);
    bool addFromJsonItem(const json &j, GameSession &session);

    json toJson(const PlaceableItem &item);
    bool addFromJsonPlaceableItem(const json &j, GameSession &session);

    json toJson(const Chair &item);
    bool addFromJsonChair(const json &j, GameSession &session);

    json toJson(const Teleporter &item);
    bool addFromJsonTeleporter(const json &j, GameSession &session);

}