#pragma once

#include <string>

namespace space
{
    class PhysicsShape
    {
        public:
            enum PhysicsShapeType { Unknown, Circle, Rectangle };
            // Fields

            // Constructor
            PhysicsShape() : _type(PhysicsShapeType::Unknown), _width(0), _height(0) { }

            // Methods
            float radius() const { return _width; }
            void radius(float radius) { _width = radius; }

            float width() const { return _width; }
            void width(float width) { _width = width; }

            float height() const { return _height; }
            void height(float height) { _height = height; }

            PhysicsShapeType type() const { return _type; }
            void type(PhysicsShapeType type) { _type = type; }

            static std::string toString(PhysicsShapeType input)
            {
                switch (input)
                {
                    case Circle: return "circle";
                    case Rectangle: return "rectangle";
                    case Unknown: return "unknown";
                    default: throw std::runtime_error("Unknown physics shape type");
                }
            }
            static PhysicsShapeType fromString(const std::string &input)
            {
                if (input == "circle") { return Circle; }
                if (input == "rectangle") { return Rectangle; }
                if (input == "unknown") { return Unknown; }
                throw std::runtime_error("Unknown physics shape type");
            }

        private:
            // Fields
            PhysicsShapeType _type;
            float _width;
            float _height;
    };
} // space