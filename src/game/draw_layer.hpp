#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"

#include "../quadtree/Quadtree.h"

namespace space
{
    class SpaceObject;
    class GameSession;
    class RenderCamera;
    class Area;
    class PlacedItem;

    class DrawLayerPlacedItemsTree : public quadtree::Quadtree<PlacedItem *>, private NonCopyable
    {
        public:
            // Fields

            // Constructor
            DrawLayerPlacedItemsTree(sf::Vector2f boxSize);

            // Methods
    };

    class DrawLayer : private NonCopyable
    {
        public:
            typedef std::vector<SpaceObject *> SpaceObjects;

            // Fields
            bool sortEveryDraw;
            const bool useQuadTree;

            // Constructor
            DrawLayer(bool useQuadTree, bool sortEveryDraw = false);

            // Methods
            void draw(GameSession &session, RenderCamera &target);

            const SpaceObjects &drawables() const { return _drawables; }

            void addObject(SpaceObject *obj);
            void removeObject(SpaceObject *obj);

            void sortObjects();

            bool checkForMouse(const Area *inRelationToArea, GameSession &session, sf::Vector2f mousePosition) const;

            const DrawLayerPlacedItemsTree &quadTree() const { return _quadtree; }

        private:
            // Fields
            SpaceObjects _drawables;
            DrawLayerPlacedItemsTree _quadtree;

            // Methods
            static bool sortByPosition(SpaceObject *obj1, SpaceObject *obj2);
    };
} // space