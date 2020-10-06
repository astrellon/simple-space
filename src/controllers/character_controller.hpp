#pragma once

#include <algorithm>
#include <vector>
#include <map>
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
            typedef std::map<Interactable &, float> InteractableMap;
            typedef std::vector<Interactable &> InteractableList;
            // Fields

            // Constructor
            CharacterController(GameSession &session);

            // Methods
            virtual std::string type() const = 0;

            virtual void update(sf::Time dt) = 0;

            Inventory &inventory() { return *_inventory.get(); }
            const Inventory &inventory() const { return *_inventory.get(); }
            void inventory(std::unique_ptr<Inventory> inventory) { _inventory = std::move(inventory); }

            void controlling(ControllingValue value) { _controlling = value; }
            ControllingValue controlling() const { return _controlling; }

            virtual void controllingShip(Ship *ship) { _ship = ship; }
            Ship *controllingShip() const { return _ship; }

            virtual void controllingCharacter(Character *character) { _character = character; }
            Character *controllingCharacter() const { return _character; }

            void teleportClone(TeleportClone *clone) { _teleportClone = clone; }
            TeleportClone* teleportClone() const { return _teleportClone; }

            void interactRangeObjects(float distance) { _interactRangeObjects = distance; }
            float interactRangeObjects() const { return _interactRangeObjects; }

            void interactRangeShips(float distance) { _interactRangeShips = distance; }
            float interactRangeShips() const { return _interactRangeShips; }

            bool canInteractWith(Interactable &item) const;
            InteractableMap &canInteractWith() { return _canInteractWith; }
            const InteractableList &canInteractWithInRange() { return _canInteractWithInRange; }
            void clearCanInteractWith() { _canInteractWith.clear(); }

            void checkForInteractables(sf::Vector2f position, const Area &area);
            void checkForInTeleportRange(sf::Vector2f position, const Area &area, std::vector<PlacedItemPair<Teleporter>> &teleportersInRange);

            void dropItem(PlaceableItem *placeableItem);

            void interactWithClosest();

        protected:
            // Fields
            GameSession &_session;
            Ship *_ship;
            Character *_character;
            TeleportClone *_teleportClone;
            ControllingValue _controlling;
            std::unique_ptr<Inventory> _inventory;
            InteractableMap _canInteractWith;
            InteractableList _canInteractWithInRange;

            float _interactRangeObjects;
            float _interactRangeShips;

            float _timeToNextIdle;
            float _dizzy;

            // Methods
            virtual void updateAnimations(sf::Time dt);
    };
} // space