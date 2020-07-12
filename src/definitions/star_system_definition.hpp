#pragma once

#include <string>
#include <memory>

#include "base_definition.hpp"
#include "celestial_body_definition.hpp"
#include "star_background_options.hpp"

namespace space
{
    class Engine;

    class StarSystemDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            StarBackgroundOptions starBackgroundOptions;

            std::unique_ptr<CelestialBodyDefinition> rootBody;

            // Constructor
            StarSystemDefinition(DefinitionId id) : BaseDefinition(id) { }
            virtual ~StarSystemDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "star-system"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space