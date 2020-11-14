#include "ui_definitions.hpp"

#include <sstream>

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../imgui/imgui.h"

#include "../definition_manager.hpp"

#include "../definitions/dialogue.hpp"
#include "../definitions/shader_definition.hpp"
#include "../definitions/character_definition.hpp"
#include "../definitions/ship_definition.hpp"
#include "../definitions/animated_texture.hpp"
#include "../definitions/celestial_body_definition.hpp"
#include "../definitions/grass_effect_definition.hpp"
#include "../definitions/item_definition.hpp"
#include "../definitions/placeable_item_definition.hpp"
#include "../definitions/planet_definition.hpp"
#include "../definitions/planet_surface_definition.hpp"
#include "../definitions/space_portal_definition.hpp"
#include "../definitions/star_system_definition.hpp"
#include "../definitions/compendium_definition.hpp"

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
            else if (type == AnimatedTexture::DefinitionType())
            {
                drawAnimatedTexture(static_cast<AnimatedTexture &>(*_selectedCopy));
            }
            else if (type == GrassEffectDefinition::DefinitionType())
            {
                drawGrassEffect(static_cast<GrassEffectDefinition &>(*_selectedCopy));
            }
            else if (type == CompendiumDefinition::DefinitionType())
            {
                drawCompendium(static_cast<CompendiumDefinition &>(*_selectedCopy));
            }
            else if (type == PlanetDefinition::DefinitionType())
            {
                drawPlanet(static_cast<PlanetDefinition &>(*_selectedCopy));
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
        std::cout << "Copied: " << copyJson << std::endl;
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
        drawCompendiumLine(character.compendiumId, character.compendiumDef);
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

        std::stringstream interiorPolygon;
        for (auto &point : ship.interiorPolygon)
        {
            interiorPolygon << "[" << point.x << ", " << point.y << "] ";
        }
        ImGui::TextWrapped("Interior Polygon: %s", interiorPolygon.str().c_str());

        ImGui::Text("Extra Polygons:");
        for (auto i = 0; i < ship.extraInteriorPolygons.size(); i++)
        {
            auto &polygon = ship.extraInteriorPolygons[i];

            std::stringstream extraInteriorPolygon;
            for (auto &point : polygon)
            {
                extraInteriorPolygon << "[" << point.x << ", " << point.y << "] ";
            }
            ImGui::TextWrapped("%d - : %s", i + 1, extraInteriorPolygon.str().c_str());
        }

        std::stringstream enginePlacements;
        ImGui::Text("Engine Glow Placements:");
        for (auto &point : ship.enginePlacements)
        {
            enginePlacements << "[" << point.x << ", " << point.y << "] ";
        }
        ImGui::TextWrapped("%s", enginePlacements.str().c_str());
    }

    void UIDefinitions::drawPlanet(PlanetDefinition &planet)
    {
        ImGui::Text("Name: %s", planet.name.c_str());
        drawCompendiumLine(planet.compendiumId, planet.compendiumDef);
        drawColour("Glow Colour:", planet.glowColour);

        ImGui::InputFloat("Rotation Rate: ", &planet.rotationRate);
        ImGui::InputFloat("Size: ", &planet.size);
        ImGui::InputFloat("Scale: ", &planet.scale);
        ImGui::InputFloat("Oscillate Noise: ", &planet.oscillateNoise);
    }

    void UIDefinitions::drawAnimatedTexture(AnimatedTexture &animated)
    {
        ImGui::Text("Texture Path: %s", animated.texturePath().c_str());
        ImGui::Text("Sprite Size: %u, %u", animated.spriteSize().x, animated.spriteSize().y);
        ImGui::Text("Sequences:");

        ImGui::TreePush();
        for (auto &sequence : animated.sequences())
        {
            ImGui::Text("Name: %s", sequence.first.c_str());
            ImGui::TreePush();
            ImGui::Text("Timing: %f", sequence.second.frameTiming);
            std::stringstream ss;

            auto first = true;
            for (auto frame : sequence.second.frames())
            {
                if (!first)
                {
                    ss << ", ";
                }
                first = false;
                ss << frame;
            }

            ImGui::Text("Frames: %s", ss.str().c_str());
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    void UIDefinitions::drawGrassEffect(GrassEffectDefinition &grassEffect)
    {
        ImGui::Text("Texture Path: %s, loaded %s", grassEffect.texturePath.c_str(), grassEffect.texture ? "true" : "false");
        ImGui::Text("Shader Id: %s loaded %s", grassEffect.shaderId.c_str(), grassEffect.shader ? "true" : "false");

        drawColour("Tip Colour", grassEffect.tipColour);
        drawColour("Side Colour", grassEffect.sideColour);
        drawColour("Wind Colour", grassEffect.windColour);
    }

    void UIDefinitions::drawColour(const char *label, sf::Color &colour)
    {
        float colourValues[4] = { colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f };
        ImGui::ColorEdit4(label, colourValues);
        colour.r = static_cast<sf::Uint8>(colourValues[0] * 255.0f);
        colour.g = static_cast<sf::Uint8>(colourValues[1] * 255.0f);
        colour.b = static_cast<sf::Uint8>(colourValues[2] * 255.0f);
        colour.a = static_cast<sf::Uint8>(colourValues[3] * 255.0f);
    }

    void UIDefinitions::drawCompendium(CompendiumDefinition &compendium)
    {
        ImGui::Text("Name: %s", compendium.name.c_str());
        ImGui::Text("Description: %s", compendium.description.c_str());
    }

    void UIDefinitions::drawCompendiumLine(const DefinitionId &id, const CompendiumDefinition *compendium)
    {
        if (id.size())
        {
            ImGui::Text("Compendium: %s, loaded: %s", id.c_str(), compendium ? "true" : "false");
        }
        else
        {
            ImGui::Text("Compendium: Not Set");
        }
    }
} // space

// Code code code code code I'm Alan and I can totally type without looking at the keyboard. Also I watch YouTube alllllll daaaaay. K byyyyyeeeeee ~~~