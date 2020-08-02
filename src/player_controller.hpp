#pragma once

#include <algorithm>
#include <vector>
#include <SFML/System.hpp>

#include "non_copyable.hpp"
#include "game/inventory.hpp"

namespace space
{
    // TODO Change to enum class
    enum ControllingValue { ControlNone, ControlShip, ControlCharacter };

    class Ship;
    class Planet;
    class Character;
    class PlacedItem;
    class PlaceableItem;
    class SpaceObject;
    class StarSystem;
    class WalkableArea;
    class GameSession;
    class TeleportClone;

    class PlayerController : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            PlayerController(GameSession &session);

            // Methods
            void update(sf::Time dt);

            Inventory &inventory() { return _inventory; }
            const Inventory &inventory() const { return _inventory; }

            void controlling(ControllingValue value) { _controlling = value; }
            ControllingValue controlling() const { return _controlling; }

            void controllingShip(Ship *ship) { _ship = ship; }
            Ship *controllingShip() const { return _ship; }

            void controllingCharacter(Character *character) { _character = character; }
            Character *controllingCharacter() const { return _character; }

            void teleportClone(TeleportClone *clone) { _teleportClone = clone; }
            TeleportClone* teleportClone() const { return _teleportClone; }

            void interactRangeObjects(float distance)
            {
                _interactRangeObjects = distance;
                _interactRangeObjectsSquared = distance * distance;
            }
            float interactRangeObjects() const { return _interactRangeObjects; }
            float interactRangeObjectsSquared() const { return _interactRangeObjectsSquared; }

            void interactRangeShips(float distance)
            {
                _interactRangeShips = distance;
                _interactRangeShipsSquared = distance * distance;
            }
            float interactRangeShips() const { return _interactRangeShips; }
            float interactRangeShipsSquared() const { return _interactRangeShipsSquared; }

            bool addCanInteractWith(PlacedItem *item);
            bool removeCanInteractWith(PlacedItem *item);
            bool canInteractWith(PlacedItem *item) const;
            std::vector<PlacedItem *> &canInteractWith() { return _canInteractWith; }
            void clearCanInteractWith() { _canInteractWith.clear(); }

            void addShipInTeleportRange(Ship *ship);
            void removeShipInTeleportRange(Ship *ship);
            bool shipInTeleportRange(Ship *ship) const;
            std::vector<Ship *> &shipsInTeleportRange() { return _shipsInTeleportRange; }
            void clearShipsInTeleportRange() { _shipsInTeleportRange.clear(); }

            void addPlanetInTeleportRange(Planet *planet);
            void removePlanetInTeleportRange(Planet *planet);
            bool planetInTeleportRange(Planet *planet) const;
            std::vector<Planet *> &planetsInTeleportRange() { return _planetsInTeleportRange; }
            void clearPlanetsInTeleportRange() { _planetsInTeleportRange.clear(); }

            void checkForGroundInteractables(sf::Vector2f position, const WalkableArea &area);
            void checkForTeleportableShips(sf::Vector2f position, const StarSystem &starSystem);
            void checkForTeleportablePlanets(sf::Vector2f position, const StarSystem &starSystem);

            void dropItem(PlaceableItem *placeableItem);

            void interactWithClosest();

        private:
            // Fields
            GameSession &_session;
            Ship *_ship;
            Character *_character;
            TeleportClone *_teleportClone;
            ControllingValue _controlling;
            Inventory _inventory;
            std::vector<PlacedItem *> _canInteractWith;
            std::vector<Ship *> _shipsInTeleportRange;
            std::vector<Planet *> _planetsInTeleportRange;

            float _interactRangeObjects;
            float _interactRangeObjectsSquared;
            float _interactRangeShips;
            float _interactRangeShipsSquared;

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space