#pragma once

#include <vector>
#include <functional>

#include <SFML/Graphics.hpp>

#include "types.hpp"

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

            // Types
            typedef std::function<void ()> GeneralAction;
            typedef std::vector<MoveSpaceObject> MoveSpaceObjectList;
            typedef std::vector<ObjectId> ObjectIdList;

            // Fields

            // Constructor
            NextFrameState();

            // Methods
            void clear();

            void addMoveSpaceObject(SpaceObject *obj, sf::Vector2f position, Area *area);
            void addRemoveObject(const ObjectId &id);
            void addAction(GeneralAction action);

            const MoveSpaceObjectList &moveSpaceObject() const { return _moveSpaceObjects; }
            const ObjectIdList &removeObjects() const { return _removeObjects; }
            void triggerActions();

        private:
            // Fields
            MoveSpaceObjectList _moveSpaceObjects;
            ObjectIdList _removeObjects;
            std::vector<GeneralAction> _generalActions;
    };
} // space