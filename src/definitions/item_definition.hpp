#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;

    class ItemDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string texturePath;
            std::string name;

            const sf::Texture *texture;

            // Constructor
            ItemDefinition(const DefinitionId &id) : BaseDefinition(id) { }
            virtual ~ItemDefinition() { }

            // Methods
            virtual void onPostLoad(Engine &engine);
    };
} // space