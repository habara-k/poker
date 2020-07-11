#ifndef POKER_AGENT_H
#define POKER_AGENT_H

#include "observable.h"

namespace poker {
    class Agent {
    public:
        Agent();

        Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions);
    };
}

#endif //POKER_AGENT_H
