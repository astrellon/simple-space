#include "game_session.hpp"

#include <SFML/OpenGL.hpp>
#include "imgui/imgui.h"

#include <valgrind/callgrind.h>

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
#include "game/live_photo.hpp"
#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "definitions/star_system_definition.hpp"
#include "definitions/planet_surface_definition.hpp"
#include "definitions/cursor.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "effects/transition.hpp"
#include "effects/teleport_screen_effect.hpp"
#include "controllers/npc_controller.hpp"
#include "game-ui/in_game_ui_page.hpp"
#include "game-ui/game_ui_manager.hpp"
#include "game-ui/in-game/in_game_main_menu_panel.hpp"

#include "serialisers/json/json_serialisers_game.hpp"
#include "serialisers/loading_context.hpp"

#include <tmxlite/Map.hpp>

namespace space
{
    void printAreaName(Area *area)
    {
        if (!area)
        {
            std::cout << "Not in an area";
            return;
        }

        auto partOfObj = area->partOfObject();
        if (!partOfObj)
        {
            std::cout << "Area not part of an object";
            return;
        }

        std::cout << partOfObj->id;
        return;
    }

    GameSession::GameSession(Engine &engine) :
        _engine(engine),
        _mouseOverObject(nullptr),
        _nextMouseOverObject(nullptr),
        _playerController(*this),
        _drawingPreTeleport(false),
        _nextId(0),
        _takingAPhoto(false)
    {
        _teleportEffect = std::make_unique<TeleportScreenEffect>();
        _teleportEffect->init(engine.definitionManager());

        _portalOverlay.init(engine.definitionManager());
    }
    GameSession::~GameSession()
    {

    }

    bool GameSession::tryGetItem(const ItemId &id, Item *&result)
    {
        auto find = _itemMap.find(id);
        if (find != _itemMap.end())
        {
            result = find->second;
            return true;
        }

        return false;
    }
    bool GameSession::tryGetSpaceObject(const ObjectId &id, SpaceObject *&result)
    {
        auto find = _spaceObjectsMap.find(id);
        if (find != _spaceObjectsMap.end())
        {
            result = find->second;
            return true;
        }

        return false;
    }

    LivePhoto *GameSession::createLivePhoto(Area &insideArea, sf::IntRect photoArea)
    {
        auto newIdNumber = nextId();
        std::stringstream newIdSS;
        newIdSS << "LIVE_PHOTO_" << newIdNumber;

        auto newId = newIdSS.str();
        auto newIdPrefix = newId + ":";

        auto parentObject = insideArea.partOfObject();
        auto rootObject = parentObject->rootObject();

        auto photoSize = static_cast<sf::Vector2u>(photoArea.getSize());
        auto result = createObject<LivePhoto>(newId, photoSize);

        CloneContext cloneContext(*this, photoArea, &insideArea, _engine.inGameTime());
        rootObject->deepClone(newIdPrefix, cloneContext);
        auto newParentObjectId = newIdPrefix + parentObject->id;

        SpaceObject *newParent;
        if (!tryGetSpaceObject(newParentObjectId, newParent))
        {
            std::cout << "Failed to find new live photo parent: " << newParentObjectId << std::endl;
            return nullptr;
        }

        IHasArea *newParentArea = dynamic_cast<IHasArea *>(newParent);
        if (!newParentArea)
        {
            std::cout << "New parent is not an IHasArea" << std::endl;
            return nullptr;
        }

        auto target = createObject<LivePhotoTarget>(newIdPrefix + ":TARGET");
        newParentArea->area().addObject(target);
        target->transform().position = sf::Vector2f(photoArea.left, photoArea.top);

        result->targetObject(target);
        result->init(_engine);

        return result;
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

    void GameSession::saveGame()
    {
        auto json = toJson(*this);
        std::ofstream output("saveGame.json");
        output << json;
        output.close();
    }

    void GameSession::update(sf::Time dt)
    {
    CALLGRIND_TOGGLE_COLLECT;
        checkNextFrameState();

        if (Keyboard::isKeyDown(sf::Keyboard::F))
        {
            if (DrawDebug::focusOnObject == nullptr)
            {
                Character *bird;
                if (tryGetSpaceObject("BIRD_CHAR", bird))
                {
                    DrawDebug::focusOnObject = bird;
                }
            }
            else
            {
                DrawDebug::focusOnObject = nullptr;
            }
        }

        if (Keyboard::isKeyDown(sf::Keyboard::Escape))
        {
            if (_takingAPhoto)
            {
                _takingAPhoto = false;
            }
            else
            {
                toggleInGameMenu();
            }
        }

        _playerController.update(dt);

        for (auto &controller : _characterControllers)
            controller->update(dt);

        for (auto &spaceObject : _spaceObjectsUpdateEveryFrame)
            spaceObject->update(*this, dt, sf::Transform::Identity);

        handleMouse(_playerController.controllingObject());

        if (_takingAPhoto)
        {
            _engine.changeCursor("CURSOR_CAMERA");
        }
        else
        {
            _engine.changeCursor("CURSOR_DEFAULT");
        }
    CALLGRIND_TOGGLE_COLLECT;
    }

    void GameSession::draw()
    {
    CALLGRIND_TOGGLE_COLLECT;
        //std::cout << "---- New Draw ----" << std::endl;
        _renderStack.clear();
        auto &sceneRender = _engine.sceneRender();

        if (_transition.get())
        {
            auto &sceneRenderTransition = _engine.sceneRenderTransition();

            auto fromPos = Utils::getPosition(_transition->fromObject.worldTransform());
            auto toPos = Utils::getPosition(_transition->toObject.worldTransform());
            drawAtObject(_transition->fromObject, fromPos, sceneRenderTransition);
            drawAtObject(_transition->toObject, toPos, sceneRender);

            sceneRenderTransition.texture().display();

            auto t = _transition->percent(_engine.inGameTime());

            _teleportEffect->draw(*this, &sceneRenderTransition.texture().getTexture(), sceneRender, t);

            if (t >= 1.0f)
            {
                clearTransition();
                clearTeleportClone();
            }
        }
        else if (DrawDebug::focusOnObject)
        {
            auto pos = Utils::getPosition(DrawDebug::focusOnObject->worldTransform());
            drawAtObject(*DrawDebug::focusOnObject, pos, sceneRender);
        }
        else
        {
            auto controllingObject = _playerController.controllingObject();
            if (controllingObject)
            {
                auto pos = Utils::getPosition(controllingObject->worldTransform());
                drawAtObject(*controllingObject, pos, sceneRender);
            }
        }
    CALLGRIND_TOGGLE_COLLECT;
    }

    void GameSession::onPostLoad(LoadingContext &context)
    {
        // Process live photos first
        for (auto &obj : _spaceObjects)
        {
            if (obj->is<LivePhoto>())
            {
                obj->onPostLoad(*this, context);
            }
        }

        for (auto &obj : _spaceObjects)
        {
            if (!obj->is<LivePhoto>())
            {
                obj->onPostLoad(*this, context);
            }
        }

        if (playerController().controlling() == space::ControlShip)
        {
            setPlayerControllingShip(playerController().controllingShip());
        }
        else if (playerController().controlling() == space::ControlCharacter)
        {
            setPlayerControllingCharacter();
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
        if (!_nextMouseOverObject)
        {
            _nextMouseOverObject = obj;
        }
    }

    void GameSession::addToUpdateEveryFrame(SpaceObject *spaceObject)
    {
        if (doesUpdateEveryFrame(spaceObject))
        {
            return;
        }

        _spaceObjectsUpdateEveryFrame.emplace_back(spaceObject);
    }

    void GameSession::removeFromUpdateEveryFrame(SpaceObject *spaceObject)
    {
        Utils::remove(_spaceObjectsUpdateEveryFrame, spaceObject);
    }

    bool GameSession::doesUpdateEveryFrame(SpaceObject *spaceObject) const
    {
        return Utils::contains(_spaceObjectsUpdateEveryFrame, spaceObject);
    }

    bool GameSession::isInGameMenuVisible() const
    {
        return _engine.gameUIManager().inGameUIPage()->inGameMainMenuPanel().visible();
    }

    void GameSession::toggleInGameMenu()
    {
        _engine.gameUIManager().inGameUIPage()->inGameMainMenuPanel().toggleVisible();
    }

    void GameSession::inGameMenuVisible(bool visible)
    {
        _engine.gameUIManager().inGameUIPage()->inGameMainMenuPanel().visible(visible);
    }

    void GameSession::createTransition(const Area *prevArea, const Area *area, TeleportClone &teleportClone)
    {
        auto windowSize = _engine.windowSize();
        auto aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
        auto duration = sf::seconds(2.2f * aspectRatio);

        auto transition = std::make_unique<Transition>(_engine.inGameTime(), duration, teleportClone, *_playerController.controllingObject());

        setTransition(transition);
    }

    void GameSession::removeSpaceObject(const ObjectId &id)
    {
        SpaceObject *obj;
        if (!tryGetSpaceObject(id, obj))
        {
            return;
        }

        auto hasArea = dynamic_cast<IHasArea *>(obj);
        if (hasArea != nullptr)
        {
            Utils::remove(_areaObjects, hasArea);
        }

        Utils::remove(_spaceObjectsUpdateEveryFrame, obj);

        if (obj->insideArea())
        {
            obj->insideArea()->removeObject(obj);
        }

        Utils::remove(_spaceObjects, obj);

        auto find = _spaceObjectsMap.find(id);
        if (find != _spaceObjectsMap.end())
        {
            _spaceObjectsMap.erase(find);
        }

        // TODO Remove star systems and planet surfaces
    }

    void GameSession::checkNextFrameState()
    {
        for (auto &moveObj : _nextFrameState.moveSpaceObject())
        {
            std::cout << "Next frame moving object: " <<  moveObj.obj->id << " from ";
            printAreaName(moveObj.obj->insideArea());
            std::cout << " to ";
            printAreaName(moveObj.area);
            std::cout << std::endl;

            moveSpaceObject(moveObj.obj, moveObj.position, moveObj.area);
        }

        _nextFrameState.triggerActions();

        _nextFrameState.clear();
    }

    bool GameSession::checkMouseSpacePortal(SpaceObject *cameraTarget, sf::Vector2f mousePosition, SpacePortal *spacePortal)
    {
        auto &sceneRender = _engine.sceneRender();

        // Don't check portal offscreen
        if (!sceneRender.camera().viewport().contains(spacePortal->transform().position))
        {
            return false;
        }

        // Bail if we can't find the target object
        SpaceObject *targetObject;
        if (!tryGetSpaceObject(spacePortal->targetObjectId, targetObject))
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

        targetStarSystem->checkForMouse(cameraTarget->insideArea(), *this, mousePosition);
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
        if (!tryGetSpaceObject(spacePortal->targetObjectId, targetObject))
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

        auto &camera = target.camera();

        camera.following(false);
        camera.center(fromPosition);
        camera.followingRotation(false);
        camera.rotation(0);

        auto insideArea = spaceObject.insideArea();
        auto renderObject = &spaceObject;
        Ship *ignoreShip = nullptr;
        auto scale = 1.0f;

        if (insideArea)
        {
            auto areaType = insideArea->type();
            insideArea->draw(*this, target);
            scale = areaType == AreaType::Ship || areaType == AreaType::PlanetSurface ? 1.0f / Utils::InsideScale : 1.0f;

            if (areaType == AreaType::Ship)
            {
                ignoreShip = insideArea->partOfShip();
                ignoreShip->disableRender = true;
                renderObject = ignoreShip->insideArea()->partOfObject();
                camera.rotation(ignoreShip->transform().rotation);
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

        camera.scale(scale);
        target.preDraw();

        _renderStack.emplace_back(spaceObject, target, _renderStack.size() + 1, renderObject);

        renderObject->draw(*this, target);

        auto starSystem = dynamic_cast<StarSystem *>(renderObject);
        if (starSystem)
        {
            for (auto spacePortal : starSystem->area().objects(SpaceObjectType::SpacePortal))
            {
                if (_renderStack.size() <= 1)
                {
                    drawSpacePortal(spacePortal->as<SpacePortal>());
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

        target.commitBatch();

        _renderStack.pop_back();
    }

    void GameSession::handleMouse(SpaceObject *target)
    {
        _nextMouseOverObject = nullptr;

        if (ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered() || !target)
        {
            _mouseOverObject = nullptr;
            return;
        }

        auto worldMousePosition = getWorldMousePosition();
        auto rootWorld = target->rootObject();

        if (_takingAPhoto)
        {
            if (Mouse::isMousePressed(sf::Mouse::Left))
            {
                auto livePhoto = createLivePhoto(*target->insideArea(), sf::IntRect(worldMousePosition.x, worldMousePosition.y, 256, 256));

                _playerController.photoAlbum().addPhoto(livePhoto);
                _playerController.compendium().processNewPhoto(livePhoto);
                _takingAPhoto = false;
                return;
            }
        }

        StarSystem *starSystem;
        PlanetSurface *planetSurface;
        if (rootWorld->tryCast(starSystem))
        {
            auto foundInPortal = false;
            for (auto spacePortal : starSystem->area().objects(SpaceObjectType::SpacePortal))
            {
                foundInPortal = checkMouseSpacePortal(target, worldMousePosition, spacePortal->as<SpacePortal>());
                if (foundInPortal)
                {
                    break;
                }
            }

            if (!foundInPortal)
            {
                starSystem->checkForMouse(target->insideArea(), *this, worldMousePosition);
            }
        }
        else if (rootWorld->tryCast(planetSurface))
        {
            planetSurface->checkForMouse(target->insideArea(), *this, worldMousePosition);
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

    sf::Vector2f GameSession::getWorldMousePosition() const
    {
        auto &sceneRender = _engine.sceneRender();
        auto mousePosition = sf::Mouse::getPosition(*_engine.window());

        return _engine.window()->mapPixelToCoords(mousePosition, sceneRender.camera().view());
    }

    sf::Vector2f GameSession::getLocalMousePosition(Area *relativeTo) const
    {
        auto worldMousePos = getWorldMousePosition();

        auto targetWorld = Utils::getPosition(relativeTo->partOfObject()->worldTransform());
        return worldMousePos - targetWorld;
    }
} // namespace space
