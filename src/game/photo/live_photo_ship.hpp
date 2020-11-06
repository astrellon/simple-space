#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../space_object.hpp"
#include "../ihas_area.hpp"
#include "../area.hpp"

namespace space
{
    class ShipDefinition;
    class EngineFlameEffect;
    class GameSession;
    class RenderCamera;

    class LivePhotoShip : public SpaceObject, public IHasArea
    {
        public:
            // Fields
            const ShipDefinition &definition;

            // Constructor
            LivePhotoShip(const ObjectId &id, const ShipDefinition &definition);

            // Methods
            static const std::string SpaceObjectType() { return "live-photo-ship"; }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual Area &area() { return _area; }
            virtual const Area &area() const { return _area; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);

        private:
            // Fields
            Area _area;
            sf::Sprite _sprite;
            sf::Sprite _interiorSprite;
            std::vector<std::unique_ptr<EngineFlameEffect>> _engineEffects;

            // Methods
    };
} // space