#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"

namespace space
{
    class Engine;
    class AnimatedTexture;

    class SpacePortalDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string animatedTextureId;

            const AnimatedTexture *texture;

            // Constructor
            SpacePortalDefinition(const DefinitionId &id) : BaseDefinition(id), texture(nullptr) { }
            virtual ~SpacePortalDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "space-portal"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space