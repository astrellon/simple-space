#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "game_session.hpp"
#include "game/items/item_type.hpp"

namespace space
{
    class EditorCameraTarget;
    class Area;

    class EditorGameSession : public GameSession
    {
        public:
            // Types

            // Fields
            PlaceableItemDefinition *creatingItem;
            ItemType creatingItemType;

            // Constructor
            EditorGameSession(Engine &engine);

            // Methods
            virtual bool isEditor() const { return true; }

            virtual void update(sf::Time dt);
            virtual void draw();
            virtual void onPostLoad(LoadingContext &context);

            void moveCameraTo(Area &area, sf::Vector2f position = sf::Vector2f());

        private:
            // Fields
            EditorCameraTarget *_cameraTarget;

            // Methods
            void handleMouseForCreate(SpaceObject *relativeTo);
    };
} // space