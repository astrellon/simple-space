#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace space
{
    class SpaceObject;
    class GameSession;
    class RenderCamera;

    class DrawLayer
    {
        public:
            typedef std::vector<SpaceObject *> SpaceObjects;

            // Fields
            bool sortEveryDraw;

            // Constructor
            DrawLayer(bool sortEveryDraw = false);

            // Methods
            void draw(GameSession &session, RenderCamera &target);

            void addObject(SpaceObject *obj);
            void removeObject(SpaceObject *obj);

            void sortObjects();

            bool checkForMouse(GameSession &session, sf::Vector2f mousePosition) const;

        private:
            // Fields
            SpaceObjects _drawables;

            // Methods
            static bool sortByPosition(SpaceObject *obj1, SpaceObject *obj2);
    };
} // space