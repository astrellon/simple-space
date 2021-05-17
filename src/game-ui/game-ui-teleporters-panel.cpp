#include "game-ui-teleporters-panel.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-button.hpp"
#include "./game-ui-teleporter.hpp"
#include "./ui-nine-slice-image-element.hpp"

#include "../game/items/teleporter.hpp"
#include "../game/teleporter_list.hpp"
#include "../engine.hpp"

namespace space
{
    void GameUITeleportersPanel::init(GameUIManager &uiManager)
    {
        UIElement::init(uiManager);
        _uiManager = &uiManager;

        _backPanel = uiManager.createElement<UINineSliceImageElement>();
        _bodyContainer = uiManager.createElement<UIElement>();

        _backPanel->nineSlice(uiManager.defaultPanelBackPanel());
        _backPanel->positionType(YGPositionTypeAbsolute);
        _backPanel->widthPercent(100);
        _backPanel->heightPercent(100);
        addChild(_backPanel);

        _bodyContainer->widthPercent(100);
        _bodyContainer->heightPercent(100);
        _bodyContainer->flexGrow(1.0f);
        _bodyContainer->flexShrink(1.0f);
        _bodyContainer->padding(8, 8, 8, 8);
        addChild(_bodyContainer);

        width(200);
        heightAuto();
        flexShrink(1.0f);
        positionType(YGPositionTypeAbsolute);
    }

    void GameUITeleportersPanel::draw(Engine &engine, RenderCamera &target)
    {
        if (_teleporters->teleporters().size() == 0)
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
        _removeOnAddTeleporter.reset();
        _removeOnRemoveTeleporter.reset();

        _teleporters = teleporters;

        if (teleporters)
        {
            for (auto teleporter : teleporters->teleporters())
            {
                addTeleporter(teleporter.teleporter);
            }

            teleporters->onAddTeleporter.connect([this](PlacedItemPair<Teleporter> teleporter)
            {
                std::cout << "Added teleporter" << std::endl;
                this->addTeleporter(teleporter);
            });
            teleporters->onRemoveTeleporter.connect([this](PlacedItemPair<Teleporter> teleporter)
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

        _bodyContainer->addChild(teleporterUI);
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
                _bodyContainer->removeChild(teleporterUI);
                _uiManager->removeElement(teleporterUI);
                break;
            }
        }
    }
} // space