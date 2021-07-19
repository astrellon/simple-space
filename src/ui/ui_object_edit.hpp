#pragma once

#include <functional>

#include <SFML/Graphics.hpp>
#include <observe/observer.h>

#include "../types.hpp"
#include "../game/area.hpp"

namespace space
{
    class SpaceObject;

    class UIObjectEdit
    {
        public:
            // Types
            typedef std::function<void(SpaceObject *)> SelectHandler;

            // Fields
            SpaceObject *spaceObject;

            // Constructor
            UIObjectEdit();

            // Methods
            void draw();

            void onSelect(SelectHandler handler);

        private:
            // Fields
            SelectHandler _selectHandler;

            // Methods
            void drawSpaceObject();
            void drawArea(Area &area);
            // void drawLayer(DrawLayer &layer);

            static const char *areaTypeString(AreaType type);

            static bool inputVector2f(const std::string &label, sf::Vector2f &input);
    };
} // space