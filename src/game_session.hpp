#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "game/space_object.hpp"
#include "game/items/item.hpp"

#include "dialogue_manager.hpp"
#include "controllers/player_controller.hpp"

namespace space
{
    class Ship;
    class Planet;
    class Engine;
    class StarSystem;
    class StarSystemDefinition;
    class PlanetSurface;
    class PlanetSurfaceDefinition;
    class Item;
    class WalkableArea;
    class Transition;
    class TransitionData;
    class TeleportScreenEffect;
    class RenderCamera;
    class TeleportClone;
    class NpcController;

    class GameSession
    {
        public:
            // Fields
            bool showTeleporters;

            // Constructor
            GameSession(Engine &engine);
            ~GameSession();

            // Methods
            StarSystem *createStarSystem(const StarSystemDefinition &definition);
            PlanetSurface *createPlanetSurface(const PlanetSurfaceDefinition &definition);
            NpcController *createNpcController();

            template <typename T, typename... TArgs>
            auto createObject(TArgs &&... args)
            {
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _spaceObjects.emplace_back(std::move(obj));

                return result;
            }

            template <typename T, typename... TArgs>
            auto createItem(TArgs &&... args)
            {
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _items.emplace_back(std::move(obj));

                return result;
            }

            bool tryGetSpaceObject(const ObjectId &id, SpaceObject **result);

            template <typename T>
            bool tryGetSpaceObject(const ObjectId &id, T **result)
            {
                SpaceObject *temp;
                if (tryGetSpaceObject(id, &temp))
                {
                    *result = dynamic_cast<T *>(temp);
                    return *result != nullptr;
                }

                return false;
            }

            void removeSpaceObject(const ObjectId &id);

            void activeStarSystem(StarSystem *starSystem)
            {
                _activeStarSystem = starSystem;
                _activePlanetSurface = nullptr;
                _playerController.clearCanInteractWith();
                _playerController.clearShipsInTeleportRange();
            }
            StarSystem *activeStarSystem() const { return _activeStarSystem; }

            void activePlanetSurface(PlanetSurface *planetSurface)
            {
                _activePlanetSurface = planetSurface;
                _activeStarSystem = nullptr;
                _playerController.clearCanInteractWith();
                _playerController.clearShipsInTeleportRange();
            }
            PlanetSurface *activePlanetSurface() const { return _activePlanetSurface; }

            bool tryGetPlanetSurface(const DefinitionId &id, PlanetSurface **result) const;

            PlayerController &playerController() { return _playerController; }

            void setPlayerControllingShip(Ship *ship);
            void setPlayerControllingCharacter();

            Engine &engine() { return _engine; }
            const Engine &engine() const { return _engine; }

            bool isControllingCharacter() const { return _playerController.controlling() == ControlCharacter; }
            Ship *getPlayerShip() const { return _playerController.controllingShip(); }
            Ship *getShipPlayerIsInsideOf() const;
            Ship *getShipPlayerCloneIsInsideOf() const;

            void moveCharacter(Character *character, sf::Vector2f position, WalkableArea *area);

            Transition *currentTransition() const { return _transition.get(); }
            void setTransition(std::unique_ptr<Transition> &transition);
            void clearTransition();

            bool drawingPreTeleport() const { return _drawingPreTeleport; }

            DialogueManager &dialogueManager() { return _dialogueManager; }

            void update(sf::Time dt);
            void draw();
            void drawUI(sf::RenderTarget &target);

        private:
            // Fields
            Engine &_engine;

            std::vector<std::unique_ptr<SpaceObject>> _spaceObjects;
            std::vector<std::unique_ptr<StarSystem>> _starSystems;
            std::vector<std::unique_ptr<PlanetSurface>> _planetSurfaces;
            std::vector<std::unique_ptr<Item>> _items;

            std::unique_ptr<TeleportScreenEffect> _teleportEffect;
            StarSystem *_activeStarSystem;
            PlanetSurface *_activePlanetSurface;
            PlayerController _playerController;
            std::vector<std::unique_ptr<NpcController>> _npcControllers;
            std::unique_ptr<Transition> _transition;
            DialogueManager _dialogueManager;
            bool _drawingPreTeleport;

            // Methods
            void applyTransitionToCamera(const TransitionData &transitionData, RenderCamera &renderCamera);
            void drawTransitionWithCamera(const TransitionData &transitionData, RenderCamera &renderCamera);

            void createTransition(const WalkableArea *prevArea, const WalkableArea *area, const TeleportClone &teleportClone, const Character *character);
            void applyAreaToTransitionData(const WalkableArea *area, TransitionData &data) const;
            void clearTeleportClone();
    };
} // town