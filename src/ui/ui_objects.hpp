#pragma once

#include "ui_window.hpp"
#include "../types.hpp"
#include "../game/area.hpp"

namespace space
{
    class Engine;
    class SpaceObject;

    class UIObjects : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIObjects();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            ObjectId _selectedObjectId;

            // Methods
            void drawSpaceObject(SpaceObject &spaceObject);
            void drawArea(Area &area);
            void drawLayer(DrawLayer &layer);

            static const char *areaTypeString(AreaType type);

            static bool inputVector2f(const std::string &label, sf::Vector2f &input);
    };
} // space