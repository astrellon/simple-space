#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "non_copyable.hpp"

namespace space
{
    class ShaderResource : private NonCopyable
    {
        public:
            // Fields
            std::string name;
            std::string fragmentPath;
            std::string vertexPath;
            std::unique_ptr<sf::Shader> shader;

            // Constructor
            ShaderResource(const std::string &name, const std::string &fragmentPath, const std::string &vertexPath, std::unique_ptr<sf::Shader> shader) :
                name(name), fragmentPath(fragmentPath), vertexPath(vertexPath), shader(std::move(shader))
            {

            }

            // Methods
    };
} // space