#include "ui_definitions.hpp"

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../imgui/imgui.h"

#include "../definition_manager.hpp"

#include "../definitions/dialogue.hpp"
#include "../definitions/shader_definition.hpp"
#include "../definitions/character_definition.hpp"
#include "../definitions/ship_definition.hpp"

#include "../serialisers/json/json_serialisers_definitions.hpp"

namespace space
{
    UIDefinitions::UIDefinitions() : UIWindow("Definitions")
    {

    }

    void UIDefinitions::doDraw(Engine &engine)
    {
        auto width = ImGui::GetWindowContentRegionWidth();
        auto middle = 0.3f;
        auto height = ImGui::GetWindowContentRegionMax().y - 30;

        ImGui::BeginChild("DefinitionList", ImVec2(width * middle, height), false);
        auto &defs = engine.definitionManager();

        auto node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        for (auto &def : defs.definitions())
        {
            ImGui::TreeNodeEx((void *)def.second.get(), node_flags, "%s", def.first.c_str());
            if (ImGui::IsItemClicked())
            {
                setSelectedCopy(def.second.get());
            }
        }

        ImGui::EndChild();

        if (_selectedCopy)
        {
            ImGui::SameLine();
            ImGui::BeginChild("SelectedDefinition", ImVec2(width * (1 - middle), height), false);
            const auto &type = _selectedCopy->type();
            ImGui::Text("%s", type.c_str());

            if (type == Dialogue::DefinitionType())
            {
                drawDialogue(static_cast<Dialogue &>(*_selectedCopy));
            }
            else if (type == ShaderDefinition::DefinitionType())
            {
                drawShader(static_cast<ShaderDefinition &>(*_selectedCopy));
            }
            else if (type == CharacterDefinition::DefinitionType())
            {
                drawCharacter(static_cast<CharacterDefinition &>(*_selectedCopy));
            }
            else if (type == ShipDefinition::DefinitionType())
            {
                drawShip(static_cast<ShipDefinition &>(*_selectedCopy));
            }
            ImGui::EndChild();
        }

        //ImGui::EndChild();
    }

    void UIDefinitions::setSelectedCopy(BaseDefinition *definition)
    {
        _selectedCopy.reset(nullptr);
        if (!definition)
        {
            return;
        }

        // Not ideal for copying, but okay for dev UI.
        auto copyJson = toJson(*definition);
        _selectedCopy = fromJsonBaseDefinition(copyJson);
    }

    void UIDefinitions::drawDialogue(Dialogue &dialogue)
    {
        for (auto i = 0; i < dialogue.text.size(); i++)
        {
            ImGui::Text("Line %i", i + 1);
            ImGui::Text("%s", dialogue.text[i].c_str());
        }
    }

    void UIDefinitions::drawShader(ShaderDefinition &shader)
    {
        ImGui::Text("Fragment Path: %s", shader.fragementPath.c_str());
        ImGui::Text("Vertex Path: %s", shader.vertexPath.c_str());
        ImGui::Text("Loaded? %s", shader.shader.getNativeHandle() ? "true" : "false");
    }

    void UIDefinitions::drawCharacter(CharacterDefinition &character)
    {
        ImGui::Text("Name: %s", character.name.c_str());
        ImGui::Text("Sprite Size: %u", character.spriteSize);
        ImGui::Text("Speed: %f", character.speed);
        ImGui::Text("Animated Texture Id: %s loaded %s", character.animatedTextureId.c_str(), character.texture ? "true" : "false");
    }

    void UIDefinitions::drawShip(ShipDefinition &ship)
    {
        ImGui::Text("Name: %s", ship.name.c_str());
        ImGui::Text("Max Rotation: %f", ship.maxRotation);
        ImGui::Text("Turn Rate: %f", ship.turnRate);
        ImGui::Text("Max Speed: %f", ship.maxSpeed);
        ImGui::Text("Acceleration: %f", ship.acceleration);

        ImGui::Text("Engine Glow Texture: %s, loaded %s", ship.engineGlowTexturePath.c_str(), ship.engineGlowTexture ? "true": "false");

        ImGui::Text("Interior Texture: %s, loaded %s", ship.interiorTexturePath.c_str(), ship.interiorTexture ? "true" : "false");
        ImGui::Text("Interior Texture Offset: %i, %i", ship.interiorTextureOffset.x, ship.interiorTextureOffset.y);
    }
} // space