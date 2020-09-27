#pragma once

#include <memory>

#include "ui_window.hpp"
#include "../definitions/base_definition.hpp"

namespace space
{
    class Engine;
    class Dialogue;
    class ShaderDefinition;
    class CharacterDefinition;
    class ShipDefinition;

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

    };
} // space