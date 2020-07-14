#ifndef POKER_ACTION_RECORD_H
#define POKER_ACTION_RECORD_H

#include <vector>

#include "action.h"
#include "stage.h"

namespace poker {
    class ActionRecord {
        const int player_id_;
        const Action action_;
    public:
        ActionRecord(int player_id, const Action& action);

        [[nodiscard]] int player_id() const;
        [[nodiscard]] Action action() const;
    };
}

#endif //POKER_ACTION_RECORD_H
