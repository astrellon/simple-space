#include "npc_action_find_food.hpp"

#include "npc_action_move.hpp"

#include "../../controllers/npc_controller.hpp"
#include "../../game/character.hpp"
#include "../../game/area.hpp"
#include "../../game/items/food_item.hpp"
#include "../../game_session.hpp"

namespace space
{
    NpcActionFindFood::NpcActionFindFood(NpcController *controller) :
        NpcAction(controller), _stage(Stage::LookingForFood), _destinationId("")
    {

    }

    void NpcActionFindFood::update(sf::Time dt)
    {
        switch (_stage)
        {
            case Stage::LookingForFood:
            {
                auto character = controller()->controllingCharacter();
                auto insideArea = character->insideArea();
                if (insideArea == nullptr)
                {
                    return;
                }

                auto pos = character->transform().position;
                std::vector<FoodSortPair> availbleFood;

                for (auto obj : insideArea->objects())
                {
                    PlacedItem *placedItem;
                    if (obj->tryCast(placedItem))
                    {
                        if (placedItem->item->type() == FoodItem::ItemType())
                        {
                            auto foodPos = placedItem->transform().position;
                            auto distance = (foodPos - pos).lengthSquared();
                            availbleFood.emplace_back(distance, placedItem);
                        }
                    }
                }

                if (availbleFood.size() == 0)
                {
                    return;
                }

                if (availbleFood.size() > 1)
                {
                    std::sort(availbleFood.begin(), availbleFood.end(), compareFoodPair);
                }

                auto &closest = availbleFood.front();
                this->_destinationId = closest.placedFood->id;
                this->_stage = Stage::MovingToFood;
                auto destinationPos = closest.placedFood->transform().position;
                this->_moveStage = std::make_unique<NpcActionMove>(controller(), destinationPos);

                break;
            }

            case Stage::MovingToFood:
            {
                _moveStage->update(dt);
                if (_moveStage->isComplete())
                {
                    _moveStage->onComplete();
                    _stage = Stage::Eating;
                }
                break;
            }

            case Stage::Eating:
            {
                PlacedItem *destination;
                if (!controller()->session().tryGetSpaceObject(_destinationId, &destination))
                {
                    std::cout << "Food gone!" << std::endl;
                    _stage = Stage::Done;
                    break;
                }

                auto foodItem = dynamic_cast<FoodItem *>(destination->item);
                if (foodItem == nullptr)
                {
                    _stage = Stage::Done;
                    break;
                }

                controller()->inIdleAnimation(false);
                controller()->controllingCharacter()->sprite().sequence("eating", true);

                foodItem->execute(*controller(), *destination);
                if (controller()->needs().hunger() > 0.8f)
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

    void NpcActionFindFood::onComplete()
    {
        controller()->inIdleAnimation(true);
        if (_moveStage.get())
        {
            _moveStage->onComplete();
        }
    }
} // space