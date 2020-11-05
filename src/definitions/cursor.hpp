#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "base_definition.hpp"

namespace space
{
    class Engine;

    class Cursor : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            std::string texturePath;
            sf::Vector2f textureOrigin;

            const sf::Texture *texture;

            // Constructor
            Cursor(const DefinitionId &id) : BaseDefinition(id), texture(nullptr) { }
            virtual ~Cursor() { }

            // Methods
            static const std::string DefinitionType() { return "cursor"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);

    };
} // space