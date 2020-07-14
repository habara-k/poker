#ifndef POKER_AGENT_H
#define POKER_AGENT_H

#include "observable.h"
#include "result.h"

namespace poker {
    class Agent {
    public:
        virtual ~Agent() = default;

        virtual Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) = 0;

        virtual void Reward(const Observable& observable) = 0;
    };

    class CheckCallAgent : public Agent {
    public:
        Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) final;

        void Reward(const Observable& observable) final;
    };

    class CheckFoldAgent : public Agent {
    public:
        Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) final;

        void Reward(const Observable& observable) final;
    };

    class UserAgent : public Agent {
    public:
        Action ChooseAction(
                const Observable &observable,
                const std::vector<ActionRange> &possible_actions) final;

        void Reward(const Observable& observable) final;
    };
}

#endif //POKER_AGENT_H
