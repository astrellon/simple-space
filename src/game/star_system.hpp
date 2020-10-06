#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <memory>

#include "../non_copyable.hpp"
#include "../definitions/star_system_definition.hpp"
#include "../star_background.hpp"
#include "space_object.hpp"
#include "area.hpp"

namespace space
{
    class GameSession;
    class RenderCamera;
    class LoadingContext;

    class StarSystem : public SpaceObject
    {
        public:
            // Fields
            const StarSystemDefinition &definition;

            // Constructor
            StarSystem(const StarSystemDefinition &definition);
            virtual ~StarSystem() { }

            // Methods
            static const std::string SpaceObjectType() { return StarSystemDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            Area &area() { return _area; }
            const Area &area () const { return _area; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransforms);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool checkForMouse(GameSession &session, sf::Vector2f mousePosition);

        private:
            // Fields
            Area _area;
            std::unique_ptr<StarBackground> _background;

            // Methods
            void createCelestialBody(GameSession &session, const CelestialBodyDefinition *bodyDefinition, const sf::Transform &parentTransform);

    };
} // space