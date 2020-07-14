#include "record.h"

#include <cassert>

namespace poker {
    Record::Record(int player_id, const Action& action)
            : player_id_(player_id), action_(action) {}

    int Record::player_id() const {
        return player_id_;
    }

    Action Record::action() const {
        return action_;
    }
}