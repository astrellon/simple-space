#include "game_ui_teleporters_panel.hpp"

#include "./game_ui_teleporter.hpp"

#include "../game_ui_manager.hpp"
#include "../ui_text_element.hpp"
#include "../ui_button.hpp"
#include "../ui_panel.hpp"

#include "../../game/items/teleporter.hpp"
#include "../../game/teleporter_list.hpp"
#include "../../engine.hpp"

namespace space
{
    void GameUITeleportersPanel::init(GameUIManager &uiManager)
    {
        UIElement::init(uiManager);
        _uiManager = &uiManager;

        _panel = uiManager.createElement<UIPanel>();
        addChild(_panel);

        width(200);
        heightAuto();
        flexShrink(1.0f);
        positionType(YGPositionTypeAbsolute);
    }

    void GameUITeleportersPanel::draw(Engine &engine, RenderCamera &target)
    {
        if (_teleporters == nullptr || _teleporters->teleporters().size() == 0)
        {
            return;
        }

        UIElement::draw(engine, target);
    }

    void GameUITeleportersPanel::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        auto screenSize = engine.renderSize();
        margin(screenSize.y / 2 - 32, 0, 0, screenSize.x / 2 - 32 - 200);

        UIElement::update(engine, dt, parentOffset);
    }

    void GameUITeleportersPanel::teleporters(const TeleporterList *teleporters)
    {
        if (teleporters == _teleporters)
        {
            return;
        }

        for (auto teleportUI : _teleporterUIs)
        {
            _panel->bodyContainer()->removeChild(teleportUI);
            _uiManager->removeElement(teleportUI);
        }
        _teleporterUIs.clear();

        _removeOnAddTeleporter.reset();
        _removeOnRemoveTeleporter.reset();

        _teleporters = teleporters;

        if (teleporters)
        {
            for (auto teleporter : teleporters->teleporters())
            {
                addTeleporter(teleporter.teleporter);
            }

            _removeOnAddTeleporter = teleporters->onAddTeleporter.createObserver([this](PlacedItemPair<Teleporter> teleporter)
            {
                std::cout << "Added teleporter" << std::endl;
                this->addTeleporter(teleporter);
            });
            _removeOnRemoveTeleporter = teleporters->onRemoveTeleporter.createObserver([this](PlacedItemPair<Teleporter> teleporter)
            {
                std::cout << "Removed teleporter" << std::endl;
                this->removeTeleporter(teleporter);
            });
        }
    }

    void GameUITeleportersPanel::addTeleporter(PlacedItemPair<Teleporter> teleporter)
    {
        auto teleporterUI = _uiManager->createElement<GameUITeleporter>();
        teleporterUI->teleporter(teleporter);

        _panel->bodyContainer()->addChild(teleporterUI);
        _teleporterUIs.push_back(teleporterUI);
    }

    void GameUITeleportersPanel::removeTeleporter(PlacedItemPair<Teleporter> teleporter)
    {
        for (auto iter = _teleporterUIs.begin(); iter != _teleporterUIs.end(); ++iter)
        {
            auto teleporterUI = *iter;
            if (teleporterUI->teleporter().item == teleporter.item)
            {
                _teleporterUIs.erase(iter);
                _panel->bodyContainer()->removeChild(teleporterUI);
                _uiManager->removeElement(teleporterUI);
                break;
            }
        }
    }
} // space