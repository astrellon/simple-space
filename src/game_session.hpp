#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "game/space_object.hpp"

#include "map-layer.hpp"

namespace space
{
    class Ship;
    class Planet;
    class Engine;
    class StarSystem;
    class StarSystemDefinition;

    class GameSession
    {
        public:
            GameSession(Engine &engine);
            ~GameSession();

            StarSystem *createStarSystem(const StarSystemDefinition &definition);

            template <typename T, typename... TArgs>
            auto createObject(TArgs &&... args)
            {
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _spaceObjects.emplace_back(std::move(obj));

                return result;
            }

            bool tryGetSpaceObject(const ObjectId &id, SpaceObject **result);

            void activeStarSystem(StarSystem *activeStarSystem) { _activeStarSystem = activeStarSystem; }
            StarSystem *activeStarSystem() const { return _activeStarSystem; }

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

        private:
            Engine &_engine;

            std::vector<std::unique_ptr<SpaceObject>> _spaceObjects;
            std::vector<std::unique_ptr<StarSystem>> _starSystems;
            std::unique_ptr<MapLayer> _mapLayer;

            StarSystem *_activeStarSystem;

    };
} // town