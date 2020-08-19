#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace space
{
    class SpaceObject;
    class GameSession;

    class DrawLayer
    {
        public:
            // Fields
            bool sortEveryDraw;

            // Constructor
            DrawLayer();

            // Methods
            void draw(GameSession &session, sf::RenderTarget &target);

            void addObject(SpaceObject *obj);
            void removeObject(SpaceObject *obj);

            void sortObjects();

        private:
            // Fields
            std::vector<SpaceObject *> _drawables;

            // Methods
            static bool sortByPosition(SpaceObject *obj1, SpaceObject *obj2);
    };
} // space