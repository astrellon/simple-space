#include "npc_action_find_bed.hpp"

#include "npc_action_move.hpp"

#include "../../controllers/npc_controller.hpp"
#include "../../game/character.hpp"
#include "../../game/area.hpp"
#include "../../game/items/bed_item.hpp"
#include "../../game_session.hpp"

namespace space
{
    NpcActionFindBed::NpcActionFindBed(NpcController *controller) :
        NpcAction(controller), _stage(Stage::LookingForBed), _destinationId(""), _timeToFindBed(0.0f)
    {

    }

    void NpcActionFindBed::update(sf::Time dt)
    {
        switch (_stage)
        {
            case Stage::LookingForBed:
            {
                auto character = controller()->controllingCharacter();
                auto insideArea = character->insideArea();
                if (insideArea == nullptr)
                {
                    return;
                }

                auto pos = character->transform().position;
                std::vector<BedSortPair> availbleBed;

                for (auto obj : insideArea->objects())
                {
                    PlacedItem *placedItem;
                    if (obj->tryCast(placedItem) && placedItem->item->is<BedItem>())
                    {
                        auto bedPos = placedItem->transform().position;
                        auto distance = (bedPos - pos).lengthSquared();
                        availbleBed.emplace_back(distance, placedItem);
                    }
                }

                if (availbleBed.size() == 0)
                {
                    return;
                }

                if (availbleBed.size() > 1)
                {
                    std::sort(availbleBed.begin(), availbleBed.end(), compareBedPair);
                }

                auto &closest = availbleBed.front();
                _destinationId = closest.placedBed->id;
                _stage = Stage::MovingToBed;
                auto destinationPos = closest.placedBed->transform().position;
                _moveStage = std::make_unique<NpcActionMove>(controller(), destinationPos);
                _timeToFindBed = Utils::randf(6.0f, 9.0f);

                break;
            }

            case Stage::MovingToBed:
            {
                _timeToFindBed -= (float)dt.asSeconds();
                if (_timeToFindBed < 0.0f)
                {
                    _moveStage->onComplete();
                    std::cout << "Failed to get to bed in time\n";
                    _stage = Stage::Done;
                    break;
                }

                _moveStage->update(dt);
                if (_moveStage->isComplete())
                {
                    _moveStage->onComplete();
                    _stage = Stage::Sleeping;
                }
                break;
            }

            case Stage::Sleeping:
            {
                PlacedItem *destination;
                if (!controller()->session().tryGetSpaceObject(_destinationId, &destination))
                {
                    std::cout << "Bed gone!" << std::endl;
                    _stage = Stage::Done;
                    break;
                }

                auto bedItem = dynamic_cast<BedItem *>(destination->item);
                if (bedItem == nullptr)
                {
                    _stage = Stage::Done;
                    break;
                }

                controller()->inIdleAnimation(false);
                controller()->controllingCharacter()->sprite().sequence("sleeping", true);

                bedItem->execute(*controller(), *destination);
                if (controller()->needs().energy() > 0.8f)
                {
                    _stage = Stage::Done;
                }
                break;
            }

            case Stage::Done:
            {
                break;
            }
        }
    }

    void NpcActionFindBed::onComplete()
    {
        controller()->inIdleAnimation(true);
        if (_moveStage.get())
        {
            _moveStage->onComplete();
        }
    }
} // space