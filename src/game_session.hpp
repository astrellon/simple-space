#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "game/space_object.hpp"
#include "game/items/item.hpp"
#include "game_session_render.hpp"

#include "dialogue_manager.hpp"
#include "controllers/player_controller.hpp"
#include "next_frame_state.hpp"
#include "effects/texture_overlay.hpp"

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
    class Area;
    class Transition;
    class TransitionData;
    class TeleportScreenEffect;
    class RenderCamera;
    class TeleportClone;
    class CharacterController;
    class SpacePortal;
    class LoadingContext;

    class GameSession
    {
        public:
            // Fields
            typedef std::vector<std::unique_ptr<SpaceObject>> SpaceObjectList;
            typedef std::vector<std::unique_ptr<Item>> ItemList;
            typedef std::vector<std::unique_ptr<CharacterController>> CharacterControllerList;
            typedef std::vector<StarSystem *> StarSystemList;
            typedef std::vector<PlanetSurface *> PlanetSurfaceList;

            // Constructor
            GameSession(Engine &engine);
            ~GameSession();

            // Methods
            const SpaceObjectList &spaceObjects() const { return _spaceObjects; }
            const StarSystemList &starSystems() const { return _starSystems; }
            const PlanetSurfaceList &planetSurfaces() const { return _planetSurfaces; }
            const ItemList &items() const { return _items; }
            const CharacterControllerList &characterControllers() const { return _characterControllers; }

            StarSystem *createStarSystem(const StarSystemDefinition &definition);
            PlanetSurface *createPlanetSurface(const PlanetSurfaceDefinition &definition);

            template <typename T, typename... TArgs>
            auto createObject(TArgs &&... args)
            {
                // We want the return type to stay as type T and not SpaceObject, so we use this round-about way.
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _spaceObjects.emplace_back(std::move(obj));

                return result;
            }

            template <typename T, typename... TArgs>
            auto createItem(TArgs &&... args)
            {
                auto item = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = item.get();
                _items.emplace_back(std::move(item));

                return result;
            }

            bool tryGetItem(const ItemId &id, Item **result);

            template <typename T>
            bool tryGetItem(const ItemId &id, T **result)
            {
                Item *temp;
                if (tryGetItem(id, &temp))
                {
                    *result = dynamic_cast<T *>(temp);
                    return *result != nullptr;
                }

                return false;
            }

            template <typename T, typename... TArgs>
            auto createCharacterController(TArgs &&... args)
            {
                auto controller = std::make_unique<T>(*this, std::forward<TArgs>(args)...);
                auto result = controller.get();
                _characterControllers.emplace_back(std::move(controller));
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

            StarSystem *activeStarSystem() const { return _activeStarSystem; }
            void activeStarSystem(StarSystem *starSystem) { _activeStarSystem = starSystem; }

            PlanetSurface *activePlanetSurface() const { return _activePlanetSurface; }
            void activePlanetSurface(PlanetSurface *planetSurface) { _activePlanetSurface = planetSurface; }

            NextFrameState &nextFrameState() { return _nextFrameState; }

            bool tryGetStarSystem(const DefinitionId &id, StarSystem **result) const;
            bool tryGetPlanetSurface(const DefinitionId &id, PlanetSurface **result) const;

            PlayerController &playerController() { return _playerController; }
            const PlayerController &playerController() const { return _playerController; }

            void setPlayerControllingShip(Ship *ship);
            void setPlayerControllingCharacter();

            Engine &engine() { return _engine; }
            const Engine &engine() const { return _engine; }

            bool isControllingCharacter() const { return _playerController.controlling() == ControlCharacter; }
            Ship *getPlayerShip() const { return _playerController.controllingShip(); }
            Ship *getShipPlayerIsInsideOf() const;
            Ship *getShipPlayerCloneIsInsideOf() const;

            void moveSpaceObject(SpaceObject *spaceObject, sf::Vector2f position, Area *area, bool queue = false);

            Transition *currentTransition() const { return _transition.get(); }
            void setTransition(std::unique_ptr<Transition> &transition);
            void clearTransition();

            bool drawingPreTeleport() const { return _drawingPreTeleport; }

            DialogueManager &dialogueManager() { return _dialogueManager; }

            void saveGame();

            void update(sf::Time dt);
            void draw();
            void onPostLoad(LoadingContext &context);

            void nextId(int id) { _nextId = id; }
            int nextId() { return ++_nextId; }

            ObjectId nextObjectId();

            void setNextMouseHover(SpaceObject *obj);

            GameSessionRender &sessionRender() { return *_renderStack.rbegin(); }

        private:
            // Fields
            Engine &_engine;
            SpaceObject *_mouseOverObject;
            SpaceObject *_nextMouseOverObject;

            SpaceObjectList _spaceObjects;
            StarSystemList _starSystems;
            PlanetSurfaceList _planetSurfaces;
            ItemList _items;
            CharacterControllerList _characterControllers;
            NextFrameState _nextFrameState;

            std::unique_ptr<TeleportScreenEffect> _teleportEffect;
            StarSystem *_activeStarSystem;
            PlanetSurface *_activePlanetSurface;
            PlayerController _playerController;
            std::unique_ptr<Transition> _transition;
            DialogueManager _dialogueManager;
            bool _drawingPreTeleport;
            TextureOverlay _portalOverlay;
            int _nextId;
            std::vector<GameSessionRender> _renderStack;

            // Methods
            void applyTransitionToCamera(const TransitionData &transitionData, RenderCamera &renderCamera);
            void drawTransitionWithCamera(const TransitionData &transitionData, RenderCamera &renderCamera);

            void createTransition(const Area *prevArea, const Area *area, TeleportClone &teleportClone);
            void applyAreaToTransitionData(const Area *area, TransitionData &data) const;
            void clearTeleportClone();

            void checkNextFrameState();
            void drawSpacePortal(SpacePortal *spacePortal);
            bool checkMouseSpacePortal(sf::Vector2f mousePosition, SpacePortal *spacePortal);

            void drawAtObject(SpaceObject &spaceObject, RenderCamera &target);
    };
} // town