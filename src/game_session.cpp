#include "game_session.hpp"

#include <SFML/OpenGL.hpp>

#include "debug/draw_debug.hpp"
#include "engine.hpp"
#include "render_camera.hpp"
#include "game/ship.hpp"
#include "game/planet.hpp"
#include "game/star_system.hpp"
#include "game/planet_surface.hpp"
#include "game/character.hpp"
#include "game/walkable_area.hpp"
#include "game/teleport_clone.hpp"
#include "game/space_portal.hpp"
#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "definitions/star_system_definition.hpp"
#include "definitions/planet_surface_definition.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "effects/transition.hpp"
#include "effects/teleport_screen_effect.hpp"
#include "controllers/npc_controller.hpp"

#include "serialisers/json/json_serialisers_game.hpp"

#include <tmxlite/Map.hpp>

namespace space
{
    GameSession::GameSession(Engine &engine) : _engine(engine), _activeStarSystem(nullptr), _playerController(*this), _drawingPreTeleport(false), _activePlanetSurface(nullptr)
    {
        _teleportEffect = std::make_unique<TeleportScreenEffect>();
        _teleportEffect->init(engine.definitionManager());

        _portalOverlay.init(engine.definitionManager());
    }
    GameSession::~GameSession()
    {

    }

    StarSystem *GameSession::createStarSystem(const StarSystemDefinition &definition)
    {
        auto &result = _starSystems.emplace_back(std::make_unique<StarSystem>(*this, definition));
        return result.get();
    }

    PlanetSurface *GameSession::createPlanetSurface(const PlanetSurfaceDefinition &definition)
    {
        auto &result = _planetSurfaces.emplace_back(std::make_unique<PlanetSurface>(*this, definition));
        return result.get();
    }
    PlanetSurface *GameSession::createPlanetSurface(const PlanetSurfaceDefinition &definition, std::unique_ptr<WalkableArea> walkableArea)
    {
        auto &result = _planetSurfaces.emplace_back(std::make_unique<PlanetSurface>(*this, definition, std::move(walkableArea)));
        return result.get();
    }

    bool GameSession::tryGetItem(const ItemId &id, Item **result)
    {
        for (auto &item : _items)
        {
            if (item->id == id)
            {
                *result = item.get();
                return true;
            }
        }

        return false;
    }
    bool GameSession::tryGetSpaceObject(const ObjectId &id, SpaceObject **result)
    {
        for (auto &obj : _spaceObjects)
        {
            if (obj->id == id)
            {
                *result = obj.get();
                return true;
            }
        }

        return false;
    }

    void GameSession::setPlayerControllingShip(Ship *ship)
    {
        _engine.spriteScale(1.0f);

        auto &sceneRenderCamera = _engine.sceneRender().camera();
        sceneRenderCamera.followingRotation(false);
        sceneRenderCamera.followingId(ship->id);
        sceneRenderCamera.scale(1.0f);

        _playerController.controllingShip(ship);
        _playerController.controlling(ControlShip);

        clearTransition();
    }
    void GameSession::setPlayerControllingCharacter()
    {
        auto scale = 1.0f / Utils::getInsideScale();
        _engine.spriteScale(scale);

        auto &sceneRenderCamera = _engine.sceneRender().camera();

        auto shipInside = getShipPlayerIsInsideOf();
        if (shipInside)
        {
            sceneRenderCamera.followingRotationId(shipInside->id);
        }
        sceneRenderCamera.followingId(_playerController.controllingCharacter()->id);
        sceneRenderCamera.scale(scale);
        _playerController.controlling(ControlCharacter);

        clearTransition();
    }

    Ship *GameSession::getShipPlayerIsInsideOf() const
    {
        const auto character = _playerController.controllingCharacter();
        if (character == nullptr)
        {
            return nullptr;
        }

        const auto area = character->insideArea();
        if (area == nullptr)
        {
            return nullptr;
        }

        return area->partOfShip();
    }

    Ship *GameSession::getShipPlayerCloneIsInsideOf() const
    {
        const auto character = _playerController.teleportClone();
        if (character == nullptr)
        {
            return nullptr;
        }

        const auto area = character->insideArea();
        if (area == nullptr)
        {
            return nullptr;
        }

        return area->partOfShip();
    }

    void GameSession::moveCharacter(Character *character, sf::Vector2f position, WalkableArea *area, bool queue)
    {
        if (queue)
        {
            _nextFrameState.addMoveCharacter(character, position, area);
            return;
        }

        auto prevArea = character->insideArea();
        if (character->insideArea() != nullptr)
        {
            character->insideArea()->removeCharacter(character);
        }

        auto prevTransform = character->transform();

        if (area != nullptr)
        {
            character->transform().position = position;
            area->addCharacter(character);
        }

        if (character == _playerController.controllingCharacter())
        {
            _playerController.clearCanInteractWith();
            if (_playerController.controlling() == ControlCharacter)
            {
                if (prevArea != nullptr)
                {
                    clearTeleportClone();

                    std::stringstream newId(character->id);
                    newId << "_TELEPORT_CLONE_";
                    newId << _engine.timeSinceStart().asMicroseconds();
                    auto teleportClone = createObject<TeleportClone>(newId.str(), *character, prevTransform);
                    _playerController.teleportClone(teleportClone);
                    prevArea->addCharacter(teleportClone);

                    createTransition(prevArea, area, *teleportClone, character);
                }

                if (area->partOfShip() != nullptr)
                {
                    _engine.sceneRender().camera().followingRotationId(area->partOfShip()->id);
                    _nextFrameState.nextStarSystem = area->partOfShip()->starSystem();
                }
                else if (area->partOfPlanetSurface() != nullptr)
                {
                    _engine.sceneRender().camera().followingRotation(false);
                    _nextFrameState.nextPlanetSurface = area->partOfPlanetSurface();
                }
            }
        }
    }

    void GameSession::moveSpaceObject(SpaceObject *obj, sf::Vector2f position, StarSystem *starSystem, bool queue)
    {
        if (queue)
        {
            _nextFrameState.addMoveSpaceObject(obj, position, starSystem);
            return;
        }

        if (obj->starSystem() != nullptr)
        {
            obj->starSystem()->removeObject(obj);
        }

        if (starSystem == nullptr)
        {
            return;
        }

        starSystem->addObject(obj);
        obj->transform().position = position;
        auto ship = dynamic_cast<Ship *>(obj);
        if (ship)
        {
            ship->prevPosition(position);
        }

        if (obj->id == _playerController.controllingShip()->id)
        {
            _nextFrameState.nextStarSystem = starSystem;
        }
    }

    void GameSession::setTransition(std::unique_ptr<Transition> &transition)
    {
        clearTransition();
        _teleportEffect->offset(Utils::randf(-1e3, 1e3));
        _transition = std::move(transition);
    }

    void GameSession::clearTransition()
    {
        std::cout << "Clearing transition" << std::endl;
        _engine.sceneRender().transitionData = nullptr;

        auto &renderTrans = _engine.sceneRenderTransition();
        renderTrans.transitionData = nullptr;

        auto cameraProps = renderTrans.camera().cameraProps();
        cameraProps.following = false;
        cameraProps.followingRotation = false;
        renderTrans.camera().cameraProps(cameraProps);

        _transition = std::move(nullptr);
    }

    void GameSession::clearTeleportClone()
    {
        if (_playerController.teleportClone() != nullptr)
        {
            removeSpaceObject(_playerController.teleportClone()->id);
            _playerController.teleportClone(nullptr);
        }
    }

    bool GameSession::tryGetStarSystem(const DefinitionId &id, StarSystem **result) const
    {
        for (auto i = _starSystems.begin(); i != _starSystems.end(); ++i)
        {
            if (i->get()->definition.id == id)
            {
                *result = i->get();
                return true;
            }
        }

        return false;
    }
    bool GameSession::tryGetPlanetSurface(const DefinitionId &id, PlanetSurface **result) const
    {
        for (auto i = _planetSurfaces.begin(); i != _planetSurfaces.end(); ++i)
        {
            if (i->get()->definition.id == id)
            {
                *result = i->get();
                return true;
            }
        }

        return false;
    }

    void GameSession::saveGame()
    {
        auto json = toJson(*this);
        std::ofstream output("saveGame.json");
        output << json;
        output.close();
    }

    void GameSession::update(sf::Time dt)
    {
        checkNextFrameState();

        if (space::Keyboard::isKeyDown(sf::Keyboard::T))
        {
            if (_playerController.controlling() == ControlShip)
            {
                setPlayerControllingCharacter();
            }
        }

        _playerController.update(dt);

        for (auto &controller : _characterControllers)
            controller->update(dt);

        for (auto &starSystem : _starSystems)
            starSystem->update(dt);

        for (auto &plantSurface : _planetSurfaces)
            plantSurface->update(dt);

        if (_transition.get())
        {
            auto &sceneRender = _engine.sceneRender();
            auto &sceneRenderTransition = _engine.sceneRenderTransition();

            applyTransitionToCamera(_transition->toData, sceneRender);
            applyTransitionToCamera(_transition->fromData, sceneRenderTransition);
        }
    }

    void GameSession::draw()
    {
        auto &sceneRender = _engine.sceneRender();
        if (_transition.get())
        {
            auto &sceneRenderTransition = _engine.sceneRenderTransition();

            _drawingPreTeleport = false;
            drawTransitionWithCamera(_transition->toData, sceneRender);

            _drawingPreTeleport = true;
            drawTransitionWithCamera(_transition->fromData, sceneRenderTransition);

            _drawingPreTeleport = false;

            sceneRenderTransition.texture().display();

            auto t = _transition->percent(_engine.timeSinceStart());

            _teleportEffect->draw(*this, &sceneRenderTransition.texture().getTexture(), sceneRender, t);

            if (t >= 1.0f)
            {
                clearTransition();
                clearTeleportClone();
            }
        }
        else
        {
            _drawingPreTeleport = false;
            if (_activeStarSystem)
            {
                _activeStarSystem->draw(sceneRender);
                for (auto obj : _activeStarSystem->objects())
                {
                    auto spacePortal = dynamic_cast<SpacePortal *>(obj);
                    if (spacePortal == nullptr)
                    {
                        continue;
                    }

                    drawSpacePortal(spacePortal);
                }
            }
            else if (_activePlanetSurface)
            {
                _activePlanetSurface->draw(sceneRender);
            }
        }
    }

    void GameSession::onPostLoad()
    {
        for (auto &spaceObject : _spaceObjects)
            spaceObject->onPostLoad(*this);

        for (auto &planetSurface : _planetSurfaces)
            planetSurface->onPostLoad(*this);
    }

    void GameSession::applyTransitionToCamera(const TransitionData &transitionData, RenderCamera &renderCamera)
    {
        renderCamera.transitionData = &transitionData;

        auto &camera = renderCamera.camera();
        camera.cameraProps(renderCamera.transitionData->cameraProps);

        if (!transitionData.cameraProps.following)
        {
            camera.center(transitionData.position);
        }
        if (!transitionData.cameraProps.followingRotation)
        {
            camera.rotation(transitionData.rotation);
        }
    }

    void GameSession::drawTransitionWithCamera(const TransitionData &transitionData, RenderCamera &renderCamera)
    {
        if (transitionData.planetSurface)
        {
            transitionData.planetSurface->draw(renderCamera);
        }
        else if (transitionData.starSystem)
        {
            transitionData.starSystem->draw(renderCamera);
        }
    }

    void GameSession::createTransition(const WalkableArea *prevArea, const WalkableArea *area, const TeleportClone &teleportClone, const Character *character)
    {
        auto windowSize = _engine.windowSize();
        auto aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

        auto transition = std::make_unique<Transition>(_engine.timeSinceStart(), sf::seconds(2.2f * aspectRatio));

        auto &fromData = transition->fromData;
        auto &toData = transition->toData;

        toData.cameraProps = fromData.cameraProps = _engine.sceneRender().camera().cameraProps();

        fromData.cameraProps.following = true;
        fromData.cameraProps.followingId = teleportClone.id;

        applyAreaToTransitionData(prevArea, fromData);
        applyAreaToTransitionData(area, toData);

        setTransition(transition);
    }

    void GameSession::applyAreaToTransitionData(const WalkableArea *area, TransitionData &data) const
    {
        if (area->partOfShip())
        {
            data.starSystem = area->partOfShip()->starSystem();
            data.ship = area->partOfShip();
            data.cameraProps.followingRotationId = area->partOfShip()->id;
            data.cameraProps.followingRotation = true;
        }
        else
        {
            data.planetSurface = area->partOfPlanetSurface();
            data.cameraProps.followingRotation = false;
        }
    }

    void GameSession::removeSpaceObject(const ObjectId &id)
    {
        SpaceObject *obj;
        if (!tryGetSpaceObject(id, &obj))
        {
            return;
        }

        if (obj->starSystem())
        {
            obj->starSystem()->removeObject(obj);
        }

        Character *character = dynamic_cast<Character *>(obj);
        if (character != nullptr)
        {
            if (character->insideArea() != nullptr)
            {
                character->insideArea()->removeCharacter(character);
            }
        }

        // Need to loop as the spaceObjects is a vector of unique_ptrs.
        for (auto iter = _spaceObjects.begin(); iter != _spaceObjects.end(); ++iter)
        {
            if (iter->get() == obj)
            {
                _spaceObjects.erase(iter);
                break;
            }
        }
    }

    void GameSession::checkNextFrameState()
    {
        for (auto &moveChar : _nextFrameState.moveCharacters())
            moveCharacter(moveChar.character, moveChar.position, moveChar.area);

        for (auto &moveObj : _nextFrameState.moveSpaceObject())
            moveSpaceObject(moveObj.obj, moveObj.position, moveObj.starSystem);

        if (_nextFrameState.nextPlanetSurface || _nextFrameState.nextStarSystem)
        {
            _playerController.clearCanInteractWith();
            _playerController.clearShipsInTeleportRange();

            if (_nextFrameState.nextPlanetSurface)
            {
                _activePlanetSurface = _nextFrameState.nextPlanetSurface;
                _activeStarSystem = nullptr;
            }
            else if (_nextFrameState.nextStarSystem)
            {
                _activeStarSystem = _nextFrameState.nextStarSystem;
                _activePlanetSurface = nullptr;
            }

        }

        _nextFrameState.clear();
    }

    void GameSession::drawSpacePortal(SpacePortal *spacePortal)
    {
        auto &sceneRender = _engine.sceneRender();
        auto &sceneRenderTransition = _engine.sceneRenderTransition();
        auto &sceneCamera = sceneRender.camera();

        // Don't render portal offscreen
        if (!sceneCamera.viewport().contains(spacePortal->transform().position))
        {
            return;
        }

        // Bail if we can't find the target object
        SpaceObject *targetObject;
        if (!tryGetSpaceObject(spacePortal->targetObjectId, &targetObject))
        {
            return;
        }

        // Also bail if we can't find the target star system.
        auto targetStarSystem = targetObject->starSystem();
        if (!targetStarSystem)
        {
            return;
        }

        auto diff = targetObject->transform().position - spacePortal->transform().position;
        auto &transitionCamera = sceneRenderTransition.camera();
        transitionCamera.cameraProps(sceneCamera.cameraProps());
        transitionCamera.center(sceneCamera.center());
        sceneRenderTransition.texture().setView(transitionCamera.view());
        sceneRenderTransition.texture().clear(sf::Color(0, 0, 0, 0));

        if (!DrawDebug::showPortalShapes)
        {
            glEnable(GL_STENCIL_TEST);

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilMask(0xFF);
            glClearStencil(0x0);
            glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            spacePortal->drawPortal(*this, sceneRenderTransition.texture(), true);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, 1, 0xFF);
            glStencilMask(0x00);

            transitionCamera.center(sceneCamera.center() + diff);
            targetStarSystem->draw(sceneRenderTransition);

            glDisable(GL_STENCIL_TEST);

            sceneRenderTransition.texture().display();

            _portalOverlay.texture(&sceneRenderTransition.texture().getTexture());
            _portalOverlay.draw(sceneRender.texture());

            spacePortal->drawPortalOutlines(*this, sceneRender.texture());
        }
        else
        {
            spacePortal->drawPortal(*this, sceneRender.texture(), false);
        }
    }
} // namespace town
