#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace space
{
    class PlanetSurface;
    class StarSystem;
    class SpaceObject;
    class Character;
    class WalkableArea;

    class NextFrameState
    {
        public:
            class MoveCharacter
            {
                public:
                    // Fields
                    Character *character;
                    sf::Vector2f position;
                    WalkableArea *area;

                    // Constructor
                    MoveCharacter(Character *character, sf::Vector2f position, WalkableArea *area) :
                        character(character), position(position), area(area) {}

                    // Methods
            };

            class MoveSpaceObject
            {
                public:
                    // Fields
                    SpaceObject *obj;
                    sf::Vector2f position;
                    StarSystem *starSystem;

                    // Constructor
                    MoveSpaceObject(SpaceObject *obj, sf::Vector2f position, StarSystem *starSystem) :
                        obj(obj), position(position), starSystem(starSystem) {}

                    // Methods
            };

            // Fields
            StarSystem *nextStarSystem;
            PlanetSurface *nextPlanetSurface;

            // Constructor
            NextFrameState();

            // Methods
            void clear();

            void addMoveCharacter(Character *character, sf::Vector2f position, WalkableArea *area);
            void addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, StarSystem *starSystem);

            const std::vector<MoveCharacter> &moveCharacters() const { return _moveCharacters; }
            const std::vector<MoveSpaceObject> &moveSpaceObject() const { return _moveSpaceObjects; }

        private:

            std::vector<MoveCharacter> _moveCharacters;
            std::vector<MoveSpaceObject> _moveSpaceObjects;
    };
} // space