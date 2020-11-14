#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "ui_window.hpp"
#include "../definitions/base_definition.hpp"

namespace space
{
    class Engine;
    class Dialogue;
    class ShaderDefinition;
    class CharacterDefinition;
    class ShipDefinition;
    class AnimatedTexture;
    class OrbitPointCelestialDefinition;
    class GrassEffectDefinition;
    class ItemDefinition;
    class PlaceableItemDefinition;
    class PlanetDefinition;
    class PlanetSurfaceDefinition;
    class SpacePortalDefinition;
    class StarSystemDefinition;
    class CompendiumDefinition;

    class UIDefinitions : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIDefinitions();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            //DefinitionId _selectedId;
            std::unique_ptr<BaseDefinition> _selectedCopy;

            // Methods
            void setSelectedCopy(BaseDefinition *definition);

            void drawDialogue(Dialogue &dialogue);
            void drawShader(ShaderDefinition &shader);
            void drawCharacter(CharacterDefinition &character);
            void drawShip(ShipDefinition &ship);
            void drawAnimatedTexture(AnimatedTexture &animated);
            void drawOrbitPoint(OrbitPointCelestialDefinition &orbitPoint);
            void drawGrassEffect(GrassEffectDefinition &grassEffect);
            void drawItem(ItemDefinition &item);
            void drawPlaceableItem(PlaceableItemDefinition &placeableItem);
            void drawPlanet(PlanetDefinition &planet);
            void drawPlanetSurface(PlanetSurfaceDefinition &planetSurface);
            void drawSpacePortal(SpacePortalDefinition &spacePortal);
            void drawStarSystem(StarSystemDefinition &starSystem);
            void drawCompendium(CompendiumDefinition &compendium);

            void drawColour(const char *label, sf::Color &colour);
            void drawCompendiumLine(const DefinitionId &id, const CompendiumDefinition *compendium);

    };
} // space