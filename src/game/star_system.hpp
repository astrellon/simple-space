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
            static const SpaceObjectType TypeValue;

            const StarSystemDefinition &definition;

            // Constructor
            StarSystem(GameSession &session, const ObjectId &id, const StarSystemDefinition &definition);
            virtual ~StarSystem() { }

            // Methods
            virtual SpaceObjectType type() const { return TypeValue; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneStarSystem(newId, context); }
            virtual SpaceObject *deepClone(const ObjectId &newIdPrefix, const CloneContext &context);
            StarSystem *cloneStarSystem(const ObjectId &newId, const CloneContext &context);

            virtual Area &area() { return _area; }
            virtual const Area &area () const { return _area; }

            void init(GameSession &session);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransforms);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool checkForMouse(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition);

            virtual bool loopOver(LoopObjectCallback callback);

        private:
            // Fields
            Area _area;
            std::unique_ptr<StarBackground> _background;

            // Methods
            void createCelestialBody(GameSession &session, const CelestialBodyDefinition *bodyDefinition, const sf::Transform &parentTransform);

    };
} // space