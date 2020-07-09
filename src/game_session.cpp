#include "game_session.hpp"

#include "engine.hpp"
#include "game/ship.hpp"
#include "game/planet.hpp"
#include "game/star_system.hpp"
#include "game/character.hpp"
#include "game/walkable_area.hpp"
#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "definitions/star_system_definition.hpp"
#include "utils.hpp"
#include "keyboard.hpp"

#include "ui/ui_interactables.hpp"
#include "ui/ui_inventory.hpp"
#include "ui/ui_teleporter.hpp"

#include <tmxlite/Map.hpp>

namespace space
{
    GameSession::GameSession(Engine &engine) : _engine(engine), _activeStarSystem(nullptr), showTeleporters(false)
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

        auto shipInside = getShipPlayerIsInsideOf();
        _engine.camera().followingRotationId(shipInside->id);
        _engine.camera().followingId(_playerController.controllingCharacter()->id);
        _engine.camera().scale(scale);
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
                auto shipInside = getShipPlayerIsInsideOf();
                _engine.camera().followingRotationId(shipInside->id);
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

        if (_mapLayer.get() == nullptr)
        {
            tmx::Map *map;
            if (_engine.resourceManager().map("data/maps/test.tmx", &map))
            {
                _mapLayer = std::make_unique<MapLayer>(*map, 0);
            }
        }
        else
        {
            // _mapLayer->update(dt);
        }

        if (_activeStarSystem)
        {
            _activeStarSystem->update(dt);
        }
    }

    void GameSession::draw(sf::RenderTarget &target)
    {
        if (_mapLayer)
        {
            sf::RenderStates states;
            //_mapLayer->draw(target, states);
            // target.draw(*_mapLayer);
        }

        if (_activeStarSystem)
        {
            _activeStarSystem->draw(target);
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
