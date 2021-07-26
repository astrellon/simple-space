#include "ui_object_create.hpp"

#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../editor_game_session.hpp"
#include "../../definition_manager.hpp"

#include "../../definitions/placeable_item_definition.hpp"
#include "../../definitions/character_definition.hpp"

namespace space
{
    UIObjectCreate::UIObjectCreate() : UIWindow("Object Create"), _objectIndex(0), _itemTypeIndex(0), _creatingObject(false)
    {

    }

    void UIObjectCreate::doDraw(Engine &engine)
    {
        static const char *ItemTypeNames[] = { "Placeable", "Teleporter", "Chair", "Bed", "Food" };
        static ItemType ItemTypes[] = { ItemType::Placeable, ItemType::Teleporter, ItemType::Chair, ItemType::Bed, ItemType::Food };

        auto session = engine.editorSession();
        if (!session)
        {
            return;
        }

        std::vector<PlaceableItemDefinition *> itemDefs;
        std::vector<CharacterDefinition *> characterDefs;
        auto &defs = engine.definitionManager();
        for (auto &kvp : defs.definitions())
        {
            auto placeableDef = dynamic_cast<PlaceableItemDefinition *>(kvp.second.get());
            if (placeableDef != nullptr)
            {
                itemDefs.push_back(placeableDef);
                continue;
            }

            auto characterDef = dynamic_cast<CharacterDefinition *>(kvp.second.get());
            if (characterDef != nullptr)
            {
                characterDefs.push_back(characterDef);
                continue;
            }
        }

        std::vector<const char *> names;
        for (auto itemDef : itemDefs)
        {
            names.push_back(itemDef->name.c_str());
        }
        for (auto charDef : characterDefs)
        {
            names.push_back(charDef->name.c_str());
        }

        auto isItem = _objectIndex < itemDefs.size();

        ImGui::Combo("Object Type", &_objectIndex, names.data(), names.size());

        if (isItem)
        {
            ImGui::Combo("Item Type", &_itemTypeIndex, ItemTypeNames, IM_ARRAYSIZE(ItemTypeNames));
        }

        if (session->creatingItem == nullptr)
        {
            if (ImGui::Button("Create"))
            {
                if (isItem)
                {
                    std::cout << "Create item: " << itemDefs[_objectIndex]->name << std::endl;
                    // _creatingObject = true;
                    session->creatingItem = itemDefs[_objectIndex];
                    session->creatingItemType = ItemTypes[_itemTypeIndex];
                }
                else
                {
                    auto charIndex = _objectIndex - itemDefs.size();
                    std::cout << "Create character: " << characterDefs[charIndex]->name << std::endl;
                    // _creatingObject = true;
                }
            }
        }
        else
        {
            if (ImGui::Button("Stop Create"))
            {
                session->creatingItem = nullptr;
            }
        }

    }
} // space