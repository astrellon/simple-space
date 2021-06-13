#pragma once

#include <queue>
#include <memory>

#include <SFML/System.hpp>

#include "character_controller.hpp"
#include "npc_needs.hpp"
#include "actions/npc_action.hpp"

namespace space
{
    class Dialogue;
    class Interaction;

    class NpcController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            NpcController(GameSession &session);

            // Methods
            static const std::string ControllerType() { return "npc"; }
            virtual std::string type() const { return ControllerType(); }

            NpcNeeds &needs() { return _needs; }

            virtual void update(sf::Time dt);

            void dialogue(const Dialogue *dialogue);
            const Dialogue *dialogue() const { return _dialogue; }

        protected:
            // Fields
            NpcNeeds _needs;
            std::queue<std::unique_ptr<NpcAction>> _highLevelActions;

            const Dialogue *_dialogue;
            Interaction *_startDialogueAction;
    };
} // space