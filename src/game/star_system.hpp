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
#include "ihas_area.hpp"

namespace space
{
    class GameSession;
    class RenderCamera;
    class LoadingContext;

    class StarSystem : public SpaceObject, public IHasArea
    {
        public:
            // Fields
            const StarSystemDefinition &definition;

            // Constructor
            StarSystem(const ObjectId &id, const StarSystemDefinition &definition);
            virtual ~StarSystem() { }

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, GameSession &session) { return cloneStarSystem(newId, session); }
            virtual SpaceObject *deepClone(const ObjectId &newIdPrefix, GameSession &session);
            StarSystem *cloneStarSystem(const ObjectId &newId, GameSession &session);

            static const std::string SpaceObjectType() { return StarSystemDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual Area &area() { return _area; }
            virtual const Area &area () const { return _area; }

            void init(GameSession &session);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransforms);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool checkForMouse(GameSession &session, sf::Vector2f mousePosition);
            virtual bool doUpdateEveryFrame() const { return true; }

        private:
            // Fields
            Area _area;
            std::unique_ptr<StarBackground> _background;

            // Methods
            void createCelestialBody(GameSession &session, const CelestialBodyDefinition *bodyDefinition, const sf::Transform &parentTransform);

    };
} // space