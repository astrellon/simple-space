#include "game_session.hpp"

#include "engine.hpp"
#include "game/ship.hpp"
#include "game/planet.hpp"
#include "game/star_system.hpp"
#include "game/planet_surface.hpp"
#include "game/character.hpp"
#include "game/walkable_area.hpp"
#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "definitions/star_system_definition.hpp"
#include "definitions/planet_surface_definition.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "effects/transition.hpp"
#include "effects/teleport_screen_effect.hpp"

#include "ui/ui_interactables.hpp"
#include "ui/ui_inventory.hpp"
#include "ui/ui_teleporter.hpp"

#include <tmxlite/Map.hpp>

namespace space
{
    GameSession::GameSession(Engine &engine) : _engine(engine), _activeStarSystem(nullptr), showTeleporters(false), _playerController(*this)
    {
        _teleportEffect = std::make_unique<TeleportScreenEffect>();
        _teleportEffect->init(engine.resourceManager());
    }
    GameSession::~GameSession()
    {

    }

    StarSystem *GameSession::createStarSystem(const StarSystemDefinition &definition)
    {
        auto starSystem = std::make_unique<StarSystem>(*this, definition);
        auto result = starSystem.get();
        _starSystems.emplace_back(std::move(starSystem));

        return result;
    }
    PlanetSurface *GameSession::createPlanetSurface(const PlanetSurfaceDefinition &definition)
    {
        auto planetSurface = std::make_unique<PlanetSurface>(*this, definition);
        auto result = planetSurface.get();
        _planetSurfaces.emplace_back(std::move(planetSurface));

        return result;
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

    void GameSession::moveCharacter(Character *character, sf::Vector2f position, WalkableArea *area)
    {
        if (character->insideArea() != nullptr)
        {
            character->insideArea()->removeCharacter(character);
        }

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
                if (area->partOfShip() != nullptr)
                {
                    _engine.sceneRender().camera().followingRotationId(area->partOfShip()->id);
                    activeStarSystem(area->partOfShip()->starSystem());
                }
                else if (area->partOfPlanetSurface() != nullptr)
                {
                    _engine.sceneRender().camera().followingRotation(false);
                    activePlanetSurface(area->partOfPlanetSurface());
                }
            }
        }
    }

    void GameSession::setTransition(std::unique_ptr<Transition> &transition)
    {
        _transition = std::move(transition);
    }
    void GameSession::setTransition(std::unique_ptr<Transition> &&transition)
    {
        _transition = std::move(transition);
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

    void GameSession::update(sf::Time dt)
    {
        if (space::Keyboard::isKeyDown(sf::Keyboard::T))
        {
            if (_playerController.controlling() == ControlShip)
            {
                setPlayerControllingCharacter();
            }
        }

        _playerController.update(dt);

        if (_activeStarSystem)
        {
            _activeStarSystem->update(dt);
        }
        if (_activePlanetSurface)
        {
            _activePlanetSurface->update(dt);
        }
    }

    void GameSession::draw()
    {
        auto &sceneRender = _engine.sceneRender();
        if (_transition.get())
        {
            auto &sceneRenderTransition = _engine.sceneRenderTransition();
            if (_transition->fromPlanetSurface) _transition->fromPlanetSurface->draw(sceneRender);
            else if (_transition->fromStarSystem) _transition->fromStarSystem->draw(sceneRender);

            if (_transition->toPlanetSurface) _transition->toPlanetSurface->draw(sceneRenderTransition);
            else if (_transition->toStarSystem) _transition->toStarSystem->draw(sceneRenderTransition);

            sceneRenderTransition.texture().display();

            sf::Texture *test;
            _engine.resourceManager().texture("data/textures/ships/ship1.png", &test);

            _teleportEffect->draw(test, sceneRender);
            // _teleportEffect->draw(&sceneRenderTransition.texture().getTexture(), sceneRender);
        }
        else
        {
            if (_activeStarSystem) _activeStarSystem->draw(sceneRender);
            else if (_activePlanetSurface) _activePlanetSurface->draw(sceneRender);
        }
    }

    void GameSession::drawUI(sf::RenderTarget &target)
    {
        if (_playerController.controlling() != ControlShip)
        {
            UIInteractables::draw(*this);
            UIInventory::draw(_playerController);
        }

        if (showTeleporters)
        {
            UITeleporter::draw(*this, _playerController.shipsInTeleportRange(), _playerController.planetsInTeleportRange());
        }
    }
} // namespace town
