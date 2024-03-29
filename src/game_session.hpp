#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "game/space_object.hpp"
#include "game/items/item.hpp"
#include "game/ihas_area.hpp"
#include "game_session_render.hpp"

#include "dialogue_manager.hpp"
#include "controllers/player_controller.hpp"
#include "next_frame_state.hpp"
#include "effects/texture_overlay.hpp"
#include "base_game_scene.hpp"

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
    class LivePhoto;

    class GameSession : public BaseGameScene
    {
        public:
            // Fields
            typedef std::vector<std::unique_ptr<SpaceObject>> SpaceObjectList;
            typedef std::vector<std::unique_ptr<Item>> ItemList;
            typedef std::vector<std::unique_ptr<CharacterController>> CharacterControllerList;

            // Constructor
            GameSession(Engine &engine);
            ~GameSession();

            // Methods
            virtual bool isEditor() const { return false; }

            const SpaceObjectList &spaceObjects() const { return _spaceObjects; }
            const ItemList &items() const { return _items; }
            const CharacterControllerList &characterControllers() const { return _characterControllers; }
            const std::vector<IHasArea *> &objectsWithArea() const { return _areaObjects; }

            template <typename T, typename... TArgs>
            auto createObject(TArgs &&... args)
            {
                // We want the return type to stay as type T and not SpaceObject, so we use this round-about way.
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _spaceObjects.emplace_back(std::move(obj));
                _spaceObjectsMap[result->id] = result;

                auto hasArea = dynamic_cast<IHasArea *>(result);
                if (hasArea != nullptr)
                {
                    _areaObjects.push_back(hasArea);
                }

                return result;
            }

            template <typename T, typename... TArgs>
            auto createItem(TArgs &&... args)
            {
                auto item = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = item.get();
                _items.emplace_back(std::move(item));
                _itemMap[result->id] = result;

                return result;
            }

            bool tryGetItem(const ItemId &id, Item *&result);

            template <typename T>
            bool tryGetItem(const ItemId &id, T *&result)
            {
                Item *temp;
                if (tryGetItem(id, temp))
                {
                    result = dynamic_cast<T *>(temp);
                    return result != nullptr;
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

            bool tryGetSpaceObject(const ObjectId &id, SpaceObject *&result);

            template <typename T>
            bool tryGetSpaceObject(const ObjectId &id, T *&result)
            {
                SpaceObject *temp;
                if (tryGetSpaceObject(id, temp))
                {
                    result = dynamic_cast<T *>(temp);
                    return result != nullptr;
                }

                return false;
            }

            void removeSpaceObject(const ObjectId &id);

            LivePhoto *createLivePhoto(Area &insideArea, sf::IntRect photoArea);

            NextFrameState &nextFrameState() { return _nextFrameState; }

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
            GameSessionRender sessionRender()
            {
                if (_renderStack.size() == 0)
                {
                    assert(false);
                }
                return _renderStack.back();
            }

            bool drawingPreTeleport() const { return _drawingPreTeleport; }

            DialogueManager &dialogueManager() { return _dialogueManager; }

            void saveGame();

            virtual void update(sf::Time dt);
            virtual void draw();
            void drawAtObject(SpaceObject &spaceObject, sf::Vector2f fromPosition, RenderCamera &target);
            virtual void onPostLoad(LoadingContext &context);

            void nextId(int id) { _nextId = id; }
            int nextId() { return ++_nextId; }
            int currentNextId() const { return _nextId; }

            void takingAPhoto(bool value) { _takingAPhoto = value; }
            bool isTakingAPhoto() const { return _takingAPhoto; }

            ObjectId nextObjectId();

            void setNextMouseHover(SpaceObject *obj);

            void addToUpdateEveryFrame(SpaceObject *spaceObject);
            void removeFromUpdateEveryFrame(SpaceObject *spaceObject);
            bool doesUpdateEveryFrame(SpaceObject *spaceObject) const;

            bool isInGameMenuVisible() const;
            void toggleInGameMenu();
            void inGameMenuVisible(bool visible);

        protected:
            // Fields
            Engine &_engine;
            SpaceObject *_mouseOverObject;
            SpaceObject *_nextMouseOverObject;

            SpaceObjectList _spaceObjects;
            std::vector<IHasArea *> _areaObjects;
            std::map<ObjectId, SpaceObject *> _spaceObjectsMap;
            std::vector<SpaceObject *> _spaceObjectsUpdateEveryFrame;
            ItemList _items;
            std::map<ItemId, Item *> _itemMap;
            CharacterControllerList _characterControllers;
            NextFrameState _nextFrameState;

            std::vector<GameSessionRender> _renderStack;
            std::unique_ptr<TeleportScreenEffect> _teleportEffect;
            PlayerController _playerController;
            std::unique_ptr<Transition> _transition;
            DialogueManager _dialogueManager;
            bool _drawingPreTeleport;
            TextureOverlay _portalOverlay;
            int _nextId;
            bool _takingAPhoto;

            // Methods
            void createTransition(const Area *prevArea, const Area *area, TeleportClone &teleportClone);
            void clearTeleportClone();

            void checkNextFrameState();
            void drawSpacePortal(SpacePortal *spacePortal);
            bool checkMouseSpacePortal(SpaceObject *cameraTarget, sf::Vector2f mousePosition, SpacePortal *spacePortal);

            void handleMouse(SpaceObject *target);

            sf::Vector2f getWorldMousePosition() const;
            sf::Vector2f getLocalMousePosition(Area *relativeTo) const;

    };
} // town