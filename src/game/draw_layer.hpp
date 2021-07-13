#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"

namespace space
{
    class SpaceObject;
    class GameSession;
    class RenderCamera;
    class Area;

    class DrawLayer : private NonCopyable
    {
        public:
            typedef std::vector<SpaceObject *> SpaceObjects;

            // Fields
            bool sortEveryDraw;

            // Constructor
            DrawLayer(bool sortEveryDraw = false);

            // Methods
            void draw(GameSession &session, RenderCamera &target);

            const SpaceObjects &drawables() const { return _drawables; }

            void addObject(SpaceObject *obj);
            void removeObject(SpaceObject *obj);

            void sortObjects();

            bool checkForMouse(const Area *inRelationToArea, GameSession &session, sf::Vector2f mousePosition) const;

        private:
            // Fields
            SpaceObjects _drawables;

            // Methods
            static bool sortByPosition(SpaceObject *obj1, SpaceObject *obj2);
    };
} // space