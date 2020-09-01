#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;

    class ShaderDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            std::string fragementPath;
            std::string vertexPath;

            sf::Shader shader;

            // Constructor
            ShaderDefinition(const DefinitionId &id) : BaseDefinition(id) { }
            virtual ~ShaderDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "shader"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space