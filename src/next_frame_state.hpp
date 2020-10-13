#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "game/space_object.hpp"

namespace space
{
    class PlanetSurface;
    class StarSystem;
    class SpaceObject;
    class Area;

    class NextFrameState
    {
        public:
            class MoveSpaceObject
            {
                public:
                    // Fields
                    SpaceObject *obj;
                    sf::Vector2f position;
                    Area *area;

                    // Constructor
                    MoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area) :
                        obj(obj), position(position), area(area) {}

                    // Methods
            };

            // Fields

            // Constructor
            NextFrameState();

            // Methods
            void clear();

            void addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area);
            void addRemoveObject(const ObjectId &id);

            const std::vector<MoveSpaceObject> &moveSpaceObject() const { return _moveSpaceObjects; }
            const std::vector<ObjectId> &removeObjects() const { return _removeObjects; }

        private:
            // Fields
            std::vector<MoveSpaceObject> _moveSpaceObjects;
            std::vector<ObjectId> _removeObjects;
    };
} // space