#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "game/space_object.hpp"
#include "game/items/item.hpp"

#include "player_controller.hpp"
#include "map_layer.hpp"

namespace space
{
    class Ship;
    class Planet;
    class Engine;
    class StarSystem;
    class StarSystemDefinition;
    class Item;
    class WalkableArea;

    class GameSession
    {
        public:
            // Constructor
            GameSession(Engine &engine);
            ~GameSession();

            // Methods
            StarSystem *createStarSystem(const StarSystemDefinition &definition);

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

            void activeStarSystem(StarSystem *activeStarSystem) { _activeStarSystem = activeStarSystem; }
            StarSystem *activeStarSystem() const { return _activeStarSystem; }

            PlayerController &playerController() { return _playerController; }

            void setPlayerControllingShip(Ship *ship);
            void setPlayerControllingCharacter();

            Engine &engine() { return _engine; }
            const Engine &engine() const { return _engine; }

            bool isControllingCharacter() const { return _playerController.controlling() == ControlCharacter; }
            Ship *getPlayerShip() const { return _playerController.controllingShip(); }
            Ship *getShipPlayerIsInsideOf() const;

            void moveCharacter(Character *character, sf::Vector2f position, WalkableArea *area);

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

        private:
            Engine &_engine;

            std::vector<std::unique_ptr<SpaceObject>> _spaceObjects;
            std::vector<std::unique_ptr<StarSystem>> _starSystems;
            std::vector<std::unique_ptr<Item>> _items;
            std::unique_ptr<MapLayer> _mapLayer;

            StarSystem *_activeStarSystem;
            PlayerController _playerController;

    };
} // town