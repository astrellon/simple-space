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
#include "game/area.hpp"
#include "game/teleport_clone.hpp"
#include "game/space_portal.hpp"
#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "definitions/star_system_definition.hpp"
#include "definitions/planet_surface_definition.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "effects/transition.hpp"
#include "effects/teleport_screen_effect.hpp"
#include "controllers/npc_controller.hpp"

#include "serialisers/json/json_serialisers_game.hpp"
#include "serialisers/loading_context.hpp"

#include <tmxlite/Map.hpp>

namespace space
{
    GameSession::GameSession(Engine &engine) : _engine(engine), _activeStarSystem(nullptr), _playerController(*this), _drawingPreTeleport(false), _activePlanetSurface(nullptr), _nextId(0), _mouseOverObject(nullptr), _nextMouseOverObject(nullptr)
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
        auto result = createObject<StarSystem>(definition);
        _starSystems.push_back(result);
        return result;
    }

    PlanetSurface *GameSession::createPlanetSurface(const PlanetSurfaceDefinition &definition)
    {
        auto result = createObject<PlanetSurface>(definition);
        _planetSurfaces.push_back(result);
        return result;
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
        _playerController.controllingShip(ship);
        _playerController.controlling(ControlShip);

        clearTransition();
    }
    void GameSession::setPlayerControllingCharacter()
    {
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

    // void GameSession::moveCharacter(Character *character, sf::Vector2f position, Area *area, bool queue)
    // {
    //     if (queue)
    //     {
    //         _nextFrameState.addMoveSpaceObject(character, position, area);
    //         return;
    //     }

    //     auto prevArea = character->insideArea();
    //     if (character->insideArea() != nullptr)
    //     {
    //         character->insideArea()->removeObject(character);
    //     }

    //     auto prevTransform = character->transform();

    //     if (area != nullptr)
    //     {
    //         character->transform().position = position;
    //         area->addObject(character);
    //     }

    //     if (character == _playerController.controllingCharacter())
    //     {
    //         _playerController.clearCanInteractWith();
    //         if (_playerController.controlling() == ControlCharacter)
    //         {
    //             if (prevArea != nullptr)
    //             {
    //                 clearTeleportClone();

    //                 std::stringstream newId(character->id);
    //                 newId << "_TELEPORT_CLONE_";
    //                 newId << _engine.timeSinceStart().asMicroseconds();
    //                 auto teleportClone = createObject<TeleportClone>(newId.str(), *character, prevTransform);
    //                 _playerController.teleportClone(teleportClone);
    //                 prevArea->addObject(teleportClone);

    //                 createTransition(prevArea, area, *teleportClone, character);
    //             }

    //             // if (area->partOfShip() != nullptr)
    //             // {
    //             //     _engine.sceneRender().camera().followingRotationId(area->partOfShip()->id);
    //             //     _nextFrameState.nextStarSystem = area->partOfShip()->starSystem();
    //             // }
    //             // else if (area->partOfPlanetSurface() != nullptr)
    //             // {
    //             //     _engine.sceneRender().camera().followingRotation(false);
    //             //     _nextFrameState.nextPlanetSurface = area->partOfPlanetSurface();
    //             // }
    //         }
    //     }
    // }

    // void GameSession::moveSpaceObject(SpaceObject *obj, sf::Vector2f position, StarSystem *starSystem, bool queue)
    // {
    //     if (queue)
    //     {
    //         _nextFrameState.addMoveSpaceObject(obj, position, starSystem);
    //         return;
    //     }

    //     // if (obj->starSystem() != nullptr)
    //     // {
    //     //     obj->starSystem()->removeObject(obj);
    //     // }

    //     if (starSystem == nullptr)
    //     {
    //         return;
    //     }

    //     // starSystem->addObject(obj);
    //     obj->transform().position = position;
    //     auto ship = dynamic_cast<Ship *>(obj);
    //     if (ship)
    //     {
    //         ship->prevPosition(position);
    //     }

    //     if (obj->id == _playerController.controllingShip()->id)
    //     {
    //         _nextFrameState.nextStarSystem = starSystem;
    //     }
    // }
    void GameSession::moveSpaceObject(SpaceObject *spaceObject, sf::Vector2f position, Area *area, bool queue)
    {
        if (queue)
        {
            _nextFrameState.addMoveSpaceObject(spaceObject, position, area);
            return;
        }

        auto prevArea = spaceObject->insideArea();
        auto prevTransform = spaceObject->transform();

        spaceObject->transform().position = position;
        area->addObject(spaceObject);

        if (spaceObject == _playerController.controllingCharacter())
        {
            _playerController.clearCanInteractWith();
            if (_playerController.controlling() == ControlCharacter)
            {
                if (prevArea != nullptr)
                {
                    clearTeleportClone();

                    std::stringstream newId(spaceObject->id);
                    newId << "_TELEPORT_CLONE_";
                    newId << _engine.timeSinceStart().asMicroseconds();
                    auto teleportClone = createObject<TeleportClone>(newId.str(), *_playerController.controllingCharacter(), prevTransform);
                    _playerController.teleportClone(teleportClone);
                    prevArea->addObject(teleportClone);

                    createTransition(prevArea, area, *teleportClone);
                }

                if (area->partOfShip() != nullptr)
                {
                    _engine.sceneRender().camera().followingRotationId(area->partOfShip()->id);
                    _nextFrameState.nextStarSystem = area->partOfShip()->insideArea()->partOfStarSystem();
                }
                else if (area->partOfPlanetSurface() != nullptr)
                {
                    _engine.sceneRender().camera().followingRotation(false);
                    _nextFrameState.nextPlanetSurface = area->partOfPlanetSurface();
                }
            }
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

        // auto &renderTrans = _engine.sceneRenderTransition();

        // auto cameraProps = renderTrans.camera().cameraProps();
        // cameraProps.following = false;
        // cameraProps.followingRotation = false;
        // renderTrans.camera().cameraProps(cameraProps);

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
        for (auto starSystem : _starSystems)
        {
            if (starSystem->definition.id == id)
            {
                *result = starSystem;
                return true;
            }
        }

        return false;
    }
    bool GameSession::tryGetPlanetSurface(const DefinitionId &id, PlanetSurface **result) const
    {
        for (auto planetSurface : _planetSurfaces)
        {
            if (planetSurface->definition.id == id)
            {
                *result = planetSurface;
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
            starSystem->update(*this, dt, sf::Transform::Identity);

        for (auto &plantSurface : _planetSurfaces)
            plantSurface->update(*this, dt, sf::Transform::Identity);

        auto &sceneRender = _engine.sceneRender();
        _nextMouseOverObject = nullptr;

        auto mousePosition = sf::Mouse::getPosition(*_engine.window());
        auto worldMousePosition = _engine.window()->mapPixelToCoords(mousePosition, sceneRender.camera().view());

        if (_activePlanetSurface)
        {
            _activePlanetSurface->checkForMouse(*this, worldMousePosition);
        }

        if (_activeStarSystem)
        {
            auto foundInPortal = false;
            for (auto spacePortal : _activeStarSystem->area().spacePortals())
            {
                foundInPortal = checkMouseSpacePortal(worldMousePosition, spacePortal);
                if (foundInPortal)
                {
                    break;
                }
            }
            // for (auto obj : _activeStarSystem->objects())
            // {
            //     if (obj->type() != SpacePortal::SpaceObjectType())
            //     {
            //         continue;
            //     }

            //     auto spacePortal = dynamic_cast<SpacePortal *>(obj);
            //     if (spacePortal == nullptr)
            //     {
            //         continue;
            //     }

            //     foundInPortal = checkMouseSpacePortal(worldMousePosition, spacePortal);
            //     // If we have something stop checking. Having multiple portals in a system will break this.
            //     if (foundInPortal)
            //     {
            //         break;
            //     }
            // }

            if (!foundInPortal)
            {
                _activeStarSystem->checkForMouse(*this, worldMousePosition);
            }
        }

        if (_nextMouseOverObject != _mouseOverObject)
        {
            // Hover changed!
            if (_mouseOverObject)
            {
                std::cout << "Left " << _mouseOverObject->id << " ";
            }
            if (_nextMouseOverObject)
            {
                std::cout << "over " << _nextMouseOverObject->id;
            }
            else
            {
                std::cout << "over nothing";
            }

            std::cout << std::endl;
        }
        _mouseOverObject = _nextMouseOverObject;

        if (Mouse::isMousePressed(sf::Mouse::Left))
        {
            _playerController.selectedObject(_mouseOverObject ? _mouseOverObject->id : "");
        }
    }

    void GameSession::draw()
    {
        //std::cout << "---- New Draw ----" << std::endl;
        _renderStack.clear();
        auto &sceneRender = _engine.sceneRender();

        if (_transition.get())
        {
            auto &sceneRenderTransition = _engine.sceneRenderTransition();

            drawAtObject(_transition->fromObject, _transition->fromObject.transform().position, sceneRenderTransition);
            drawAtObject(_transition->toObject, _transition->toObject.transform().position, sceneRender);

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
            auto controllingObject = _playerController.controllingObject();
            if (controllingObject)
            {
                drawAtObject(*controllingObject, controllingObject->transform().position, sceneRender);
            }
        }
    }

    void GameSession::onPostLoad(LoadingContext &context)
    {
        for (auto i = 0; i < _spaceObjects.size(); i++)
        {
            auto &spaceObject = _spaceObjects[i];
            spaceObject->onPostLoad(*this, context);
        }
    }

    ObjectId GameSession::nextObjectId()
    {
        std::stringstream ss("_ID_");
        ss << nextId();
        return ss.str();
    }

    void GameSession::setNextMouseHover(SpaceObject *obj)
    {
        _nextMouseOverObject = obj;
    }

    void GameSession::createTransition(const Area *prevArea, const Area *area, TeleportClone &teleportClone)
    {
        auto windowSize = _engine.windowSize();
        auto aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
        auto duration = sf::seconds(2.2f * aspectRatio);

        auto transition = std::make_unique<Transition>(_engine.timeSinceStart(), duration, teleportClone, *_playerController.controllingObject());

        setTransition(transition);
    }

    void GameSession::removeSpaceObject(const ObjectId &id)
    {
        SpaceObject *obj;
        if (!tryGetSpaceObject(id, &obj))
        {
            return;
        }

        if (obj->insideArea())
        {
            obj->insideArea()->removeObject(obj);
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

        // TODO Remove star systems and planet surfaces
    }

    void GameSession::checkNextFrameState()
    {
        for (auto &moveObj : _nextFrameState.moveSpaceObject())
            moveSpaceObject(moveObj.obj, moveObj.position, moveObj.area);

        if (_nextFrameState.nextPlanetSurface || _nextFrameState.nextStarSystem)
        {
            _playerController.clearCanInteractWith();
            //_playerController.clearShipsInTeleportRange();

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

    bool GameSession::checkMouseSpacePortal(sf::Vector2f mousePosition, SpacePortal *spacePortal)
    {
        auto &sceneRender = _engine.sceneRender();

        // Don't check portal offscreen
        if (!sceneRender.camera().viewport().contains(spacePortal->transform().position))
        {
            return false;
        }

        // Bail if we can't find the target object
        SpaceObject *targetObject;
        if (!tryGetSpaceObject(spacePortal->targetObjectId, &targetObject))
        {
            return false;
        }

        // Also bail if we can't find the target star system.
        auto targetStarSystem = targetObject->insideArea()->partOfStarSystem();
        if (!targetStarSystem)
        {
            return false;
        }

        if (!spacePortal->isMouseOverPortal(mousePosition))
        {
            return false;
        }

        auto diff = targetObject->transform().position - spacePortal->transform().position;
        mousePosition += diff;

        targetStarSystem->checkForMouse(*this, mousePosition);
        return true;
    }

    void GameSession::drawSpacePortal(SpacePortal *spacePortal)
    {
        auto currentRenderContext = sessionRender();
        auto &sceneCamera = currentRenderContext.renderTarget().camera();

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
        // auto targetStarSystem = targetObject->starSystem();
        if (!targetObject)
        {
            return;
        }

        // Make sure we have a new render camera for the portal to draw to.
        auto renderTargetEntry = _engine.renderCameras().get();
        if (!renderTargetEntry.isValid())
        {
            return;
        }

        if (_renderStack.size() > 1)
        {
            auto &prevContext = _renderStack[_renderStack.size() - 3];
            if (prevContext.prevPortalTarget() == targetObject)
            {
                return;
            }
        }

        std::cout << "Drawing space portal: " << spacePortal->id << " " << currentRenderContext.portalLevel() << std::endl;

        auto &renderTarget = *renderTargetEntry.value;
        auto diff = targetObject->transform().position - spacePortal->transform().position;
        auto &transitionCamera = renderTarget.camera();
        transitionCamera.cameraProps(sceneCamera.cameraProps());
        transitionCamera.center(sceneCamera.center());
        renderTarget.texture().setView(transitionCamera.view());
        renderTarget.texture().clear(sf::Color(0, 0, 0, 0));

        if (!DrawDebug::showPortalShapes)
        {
            glEnable(GL_STENCIL_TEST);

            glStencilFunc(GL_ALWAYS, currentRenderContext.portalLevel(), 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilMask(0xFF);
            glClearStencil(0x0);
            glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            spacePortal->drawPortal(*this, renderTarget, true);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, currentRenderContext.portalLevel(), 0xFF);
            glStencilMask(0x00);

            drawAtObject(*targetObject, sceneCamera.center() + diff, renderTarget);

            glDisable(GL_STENCIL_TEST);

            renderTarget.texture().display();

            spacePortal->drawPortalOutlines(*this, currentRenderContext.renderTarget().texture());
        }
        else
        {
            spacePortal->drawPortal(*this, renderTarget, false);
        }

        _portalOverlay.texture(&renderTarget.texture().getTexture());
        _portalOverlay.draw(currentRenderContext.renderTarget().texture());
    }

    void GameSession::drawAtObject(SpaceObject &spaceObject, sf::Vector2f fromPosition, RenderCamera &target)
    {
        if (_renderStack.size() > 1)
        {
            std::cout << "portal overflow\n";
            return;
        }

        target.camera().following(false);
        target.camera().center(fromPosition);

        auto insideArea = spaceObject.insideArea();
        auto renderObject = &spaceObject;
        Ship *ignoreShip = nullptr;
        auto scale = 1.0f;

        if (insideArea)
        {
            auto areaType = insideArea->type();
            insideArea->draw(*this, target);
            scale = areaType == AreaType::Ship || areaType == AreaType::PlanetSurface ? 1.0f / Utils::getInsideScale() : 1.0f;

            if (areaType == AreaType::Ship)
            {
                ignoreShip = insideArea->partOfShip();
                ignoreShip->disableRender = true;
                renderObject = ignoreShip->insideArea()->partOfObject();
            }
            else
            {
                renderObject = insideArea->partOfObject();
            }
        }
        else
        {
            std::cout << "Rendering object not inside anything" << std::endl;
        }

        if (!renderObject)
        {
            return;
        }

        target.camera().scale(scale);
        target.preDraw();

        auto &renderContext = _renderStack.emplace_back(spaceObject, target, _renderStack.size() + 1, renderObject);

        renderObject->draw(*this, target);

        auto starSystem = dynamic_cast<StarSystem *>(renderObject);
        if (starSystem)
        {
            for (auto spacePortal : starSystem->area().spacePortals())
            {
                if (_renderStack.size() <= 1)
                {
                    drawSpacePortal(spacePortal);
                }
            }
        }

        if (ignoreShip)
        {
            ignoreShip->disableRender = false;
            ignoreShip->draw(*this, target);
            _engine.overlay().draw(target.texture(), 0.3);
            ignoreShip->drawInterior(*this, target);
        }

        _renderStack.pop_back();
    }
} // namespace space

