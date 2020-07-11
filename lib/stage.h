#ifndef POKER_STAGE_H
#define POKER_STAGE_H

namespace poker {
    enum class Stage {
        kPreFlop, kFlop, kTurn, kRiver, kShowdown, kEndHidden
    };
}

#endif //POKER_STAGE_H
