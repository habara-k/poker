#include "agent.h"

#include <cassert>

namespace poker {
    Agent::Agent() {}

    Action Agent::ChooseAction(
            const Observable& observable,
            const std::vector<ActionRange>& possible_actions) {
        for (const auto& action_range : possible_actions) {
            if (action_range.type() == ActionType::kCall) {
                return Action(ActionType::kCall);
            }
            else if (action_range.type() == ActionType::kFold) {
                return Action(ActionType::kFold);
            }
        }
        assert(false);
    }
}