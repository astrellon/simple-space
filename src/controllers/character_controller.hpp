#pragma once

#include <algorithm>
#include <vector>
#include <SFML/System.hpp>

#include "../non_copyable.hpp"
#include "../game/inventory.hpp"

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
    class Interactable;

    class CharacterController : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            CharacterController(GameSession &session);

            // Methods
            virtual std::string type() const = 0;

            virtual void update(sf::Time dt) = 0;

            Inventory &inventory() { return _inventory; }
            const Inventory &inventory() const { return _inventory; }

            void controlling(ControllingValue value) { _controlling = value; }
            ControllingValue controlling() const { return _controlling; }

            virtual void controllingShip(Ship *ship) { _ship = ship; }
            Ship *controllingShip() const { return _ship; }

            virtual void controllingCharacter(Character *character) { _character = character; }
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

            bool addCanInteractWith(Interactable *item);
            bool removeCanInteractWith(Interactable *item);
            bool canInteractWith(Interactable *item) const;
            std::vector<Interactable *> &canInteractWith() { return _canInteractWith; }
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

        protected:
            // Fields
            GameSession &_session;
            Ship *_ship;
            Character *_character;
            TeleportClone *_teleportClone;
            ControllingValue _controlling;
            Inventory _inventory;
            std::vector<Interactable *> _canInteractWith;
            std::vector<Ship *> _shipsInTeleportRange;
            std::vector<Planet *> _planetsInTeleportRange;

            float _interactRangeObjects;
            float _interactRangeObjectsSquared;
            float _interactRangeShips;
            float _interactRangeShipsSquared;

            // Methods
            void checkInRangeOfInteractable(Interactable *interactable);
    };
} // space