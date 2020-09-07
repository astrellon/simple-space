#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "../non_copyable.hpp"
#include "../definitions/star_system_definition.hpp"
#include "../star_background.hpp"

namespace space
{
    typedef std::string StarSystemId;
    class Engine;
    class SpaceObject;
    class GameSession;
    class RenderCamera;

    class StarSystem : private NonCopyable
    {
        public:
            typedef std::function<void(SpaceObject *)> FindObjectCallback;
            // Fields
            const StarSystemDefinition &definition;

            // Constructor
            StarSystem(GameSession &session, const StarSystemDefinition &definition);

            // Methods
            const std::vector<SpaceObject *> &objects() const { return _objects; }
            void getObjectsNearby(float radius, const sf::Vector2f &position, FindObjectCallback callback) const;
            void getObjectsNearby(float radius, const sf::Vector2f &position, std::vector<SpaceObject *> &result) const;

            void initFromDefinition();
            void addObject(SpaceObject *object);
            void removeObject(SpaceObject *object);

            void update(sf::Time dt);
            void draw(RenderCamera &target);

        private:
            // Fields
            std::vector<SpaceObject *> _objects;
            GameSession &_session;
            std::unique_ptr<StarBackground> _background;

            // Methods
            void createCelestialBody(const CelestialBodyDefinition *bodyDefinition);

    };
} // space