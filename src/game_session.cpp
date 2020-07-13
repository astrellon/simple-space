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

#include "ui/ui_interactables.hpp"
#include "ui/ui_inventory.hpp"
#include "ui/ui_teleporter.hpp"

#include <tmxlite/Map.hpp>

namespace space
{
    GameSession::GameSession(Engine &engine) : _engine(engine), _activeStarSystem(nullptr), showTeleporters(false), _playerController(*this)
    {

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
        _engine.camera().followingRotation(false);
        _engine.camera().followingId(ship->id);
        _engine.camera().scale(1.0f);
        _playerController.controllingShip(ship);
        _playerController.controlling(ControlShip);
    }
    void GameSession::setPlayerControllingCharacter()
    {
        auto scale = 1.0f / Utils::getInsideScale();
        _engine.spriteScale(scale);

        auto &camera = _engine.camera();

        auto shipInside = getShipPlayerIsInsideOf();
        if (shipInside)
        {
            camera.followingRotationId(shipInside->id);
        }
        camera.followingId(_playerController.controllingCharacter()->id);
        camera.scale(scale);
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
                    _engine.camera().followingRotationId(area->partOfShip()->id);
                    activeStarSystem(area->partOfShip()->starSystem());
                }
                else if (area->partOfPlanet() != nullptr)
                {
                    _engine.camera().followingRotation(false);
                    activePlanetSurface(area->partOfPlanet());
                }
            }
        }
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

    void GameSession::draw(sf::RenderTarget &target)
    {
        if (_activeStarSystem)
        {
            _activeStarSystem->draw(target);
        }
        if (_activePlanetSurface)
        {
            _activePlanetSurface->draw(target);
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
            UITeleporter::draw(*this, _playerController.shipsInTeleportRange());
        }
    }
} // namespace town
