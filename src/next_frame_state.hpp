#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

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
            StarSystem *nextStarSystem;
            PlanetSurface *nextPlanetSurface;

            // Constructor
            NextFrameState();

            // Methods
            void clear();

            void addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area);

            const std::vector<MoveSpaceObject> &moveSpaceObject() const { return _moveSpaceObjects; }

        private:

            std::vector<MoveSpaceObject> _moveSpaceObjects;
    };
} // space