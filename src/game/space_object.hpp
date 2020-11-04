#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../space_transform.hpp"
#include "../definitions/base_definition.hpp"
#include "../types.hpp"
#include "draw_layers.hpp"
#include "interactions/interactable.hpp"
#include "clone_context.hpp"

namespace space
{
    class GameSession;
    class LoadingContext;
    class Area;
    class RenderCamera;
    class CompendiumDefinition;

    class SpaceObject : private NonCopyable
    {
        public:
            // Fields
            const ObjectId id;

            // Constructor
            SpaceObject(const ObjectId &id) : id(id), _insideArea(nullptr), _interactable(this) { }
            virtual ~SpaceObject() { }

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) = 0;
            virtual SpaceObject *deepClone(const ObjectId &newIdPrefix, const CloneContext &context) { return clone(newIdPrefix + id, context); }
            virtual std::string type() const = 0;

            const SpaceTransform &transform() const { return _transform; }
            SpaceTransform &transform() { return _transform; }
            void transform(const SpaceTransform &transform) { _transform = transform; }
            void transform(const SpaceTransform &&transform) { _transform = transform; }
            const sf::Transform &worldTransform() const { return _worldTransform; }

            Interactable &interactable() { return _interactable; }

            virtual void insideArea(Area *area) { _insideArea = area; }
            virtual Area *insideArea() const { return _insideArea; }

            virtual void prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) { }
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) = 0;
            virtual void draw(GameSession &session, RenderCamera &target) = 0;
            virtual void onPostLoad(GameSession &session, LoadingContext &context) { }
            virtual bool doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const { return false; }
            virtual bool isGenerated() const { return false; }
            virtual bool doUpdateEveryFrame() const { return false; }

            virtual const CompendiumDefinition *compendiumDefinition() const { return nullptr; }

            virtual DrawLayers::Type drawLayer() const { return DrawLayers::Main; }

            SpaceObject *rootObject();

        protected:
            // Fields
            SpaceTransform _transform;
            sf::Transform _worldTransform;
            Area *_insideArea;
            Interactable _interactable;

            // Methods
            inline void updateWorldTransform(const sf::Transform &parentTransform)
            {
                _worldTransform = parentTransform * _transform.getTransform();
            }
    };
} // space