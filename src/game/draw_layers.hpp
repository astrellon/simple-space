#pragma once

#include <string>
#include <stdexcept>

namespace space
{
    class DrawLayers
    {
        public:
            enum Type { NotSet, Background, Main, Foreground };
            // Fields

            // Methods
            static std::string toString(Type input)
            {
                switch (input)
                {
                    case Background: return "background";
                    case Main: return "main";
                    case Foreground: return "foreground";
                    case NotSet: return "notset";
                    default: throw std::runtime_error("Unknown draw layer");
                }
            }
            static Type fromString(const std::string &input)
            {
                if (input == "background") { return Background; }
                if (input == "main") { return Main; }
                if (input == "foreground") { return Foreground; }
                if (input == "notset") { return NotSet; }
                throw std::runtime_error("Unknown draw layer");
            }

        private:
            // Constructor
            DrawLayers();
    };
} // space