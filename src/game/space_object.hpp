#pragma once

#include <string>
#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../space_transform.hpp"
#include "../definitions/base_definition.hpp"
#include "../types.hpp"
#include "draw_layers.hpp"
#include "interactions/interactable.hpp"
#include "clone_context.hpp"
#include "space_object_type.hpp"

namespace space
{
    class GameSession;
    class LoadingContext;
    class Area;
    class RenderCamera;
    class CompendiumDefinition;
    class LivePhoto;
    class CloneContext;

    class SpaceObject : private NonCopyable
    {
        public:
            // Fields
            const ObjectId id;

            // Constructor
            SpaceObject(const ObjectId &id) : id(id), _insideArea(nullptr), _partOfLivePhoto(nullptr) { }
            virtual ~SpaceObject() { }

            // Methods
            virtual SpaceObjectType type() const { return SpaceObjectType::Unknown; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) = 0;
            virtual SpaceObject *deepClone(const ObjectId &newIdPrefix, const CloneContext &context) { return clone(newIdPrefix + id, context); }

            const SpaceTransform &transform() const { return _transform; }
            SpaceTransform &transform() { return _transform; }
            void transform(const SpaceTransform &transform) { _transform = transform; }
            void transform(const SpaceTransform &&transform) { _transform = transform; }
            const sf::Transform &worldTransform() const { return _worldTransform; }

            Interactable *interactable() { return _interactable.get(); }

            virtual void insideArea(Area *area) { _insideArea = area; }
            virtual Area *insideArea() const { return _insideArea; }

            virtual void partOfLivePhoto(LivePhoto *livePhoto) { _partOfLivePhoto = livePhoto; }
            virtual LivePhoto *partOfLivePhoto() const { return _partOfLivePhoto; }

            virtual void prePhysics(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) { }
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) = 0;
            virtual void draw(GameSession &session, RenderCamera &target) = 0;
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool doesMouseHover(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition) const { return false; }
            virtual bool isGenerated() const { return false; }

            virtual const CompendiumDefinition *compendiumDefinition() const { return nullptr; }

            virtual DrawLayers::Type drawLayer() const { return DrawLayers::Main; }

            SpaceObject *rootObject();

            template <typename T>
            bool tryCast(const T *& result) const
            {
                if (type() == T::TypeValue)
                {
                    result = reinterpret_cast<const T *>(this);
                    return true;
                }
                return false;
            }

            template <typename T>
            bool tryCast(T *& result)
            {
                if (type() == T::TypeValue)
                {
                    result = reinterpret_cast<T *>(this);
                    return true;
                }
                return false;
            }

            template <typename T>
            T *as()
            {
                if (type() == T::TypeValue)
                {
                    return reinterpret_cast<T *>(this);
                }

                throw std::runtime_error("Cannot cast space object!");
            }

            template <typename T>
            const T *as() const
            {
                if (type() == T::TypeValue)
                {
                    return reinterpret_cast<const T *>(this);
                }

                throw std::runtime_error("Cannot cast space object!");
            }

            template <typename T>
            bool is() const
            {
                return type() == T::TypeValue;
            }

            virtual bool loopOver(LoopObjectCallback callback);

        protected:
            // Fields
            SpaceTransform _transform;
            sf::Transform _worldTransform;
            Area *_insideArea;
            LivePhoto *_partOfLivePhoto;
            std::unique_ptr<Interactable> _interactable;

            // Methods
            inline void updateWorldTransform(const sf::Transform &parentTransform)
            {
                _worldTransform = parentTransform * _transform.getTransform();
            }

            void populateCloneFromThis(SpaceObject *newClone, const CloneContext &context);
            void createInteractable();
    };
} // space