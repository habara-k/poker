#ifndef POKER_RECORD_H
#define POKER_RECORD_H

#include <vector>

#include "action.h"
#include "stage.h"

namespace poker {
    class Record {
        const int player_id_;
        const Action action_;
    public:
        Record(int player_id, const Action& action);

        [[nodiscard]] int player_id() const;
        [[nodiscard]] Action action() const;
    };
}

#endif //POKER_RECORD_H
