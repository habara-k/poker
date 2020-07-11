#ifndef POKER_TRAJECTORY_H
#define POKER_TRAJECTORY_H

#include <vector>

#include "action.h"

namespace poker {
    class ActionRecord {
        int player_id_;
        Action action_;
    public:
        ActionRecord(int player_id, const Action& action);

        [[nodiscard]] int player_id() const;
        [[nodiscard]] Action action() const;
    };
}

#endif //POKER_TRAJECTORY_H
