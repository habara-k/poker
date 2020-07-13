#ifndef POKER_AGENT_H
#define POKER_AGENT_H

#include "observable.h"

namespace poker {
    class Agent {
    public:
        Agent();
        virtual ~Agent() = default;

        virtual Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) = 0;
    };

    class CheckCallAgent : public Agent {
    public:
        CheckCallAgent();
        Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) final;
    };

    class UserAgent : public Agent {
    public:
        UserAgent();
        Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) final;
    };
}

#endif //POKER_AGENT_H
