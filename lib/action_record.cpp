#include "action_record.h"

#include <cassert>

namespace poker {
    ActionRecord::ActionRecord(int player_id, const Action& action)
            : player_id_(player_id), action_(action) {}

    int ActionRecord::player_id() const {
        return player_id_;
    }

    Action ActionRecord::action() const {
        return action_;
    }
}