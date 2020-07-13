#ifndef POKER_ACTION_H
#define POKER_ACTION_H

#include <optional>
#include <string>

namespace poker {
    enum ActionType {
        kCheck, kFold, kCall, kBet, kRaise, kAllIn
    };

    class Action {
        ActionType type_;
        std::optional<int> size_;
        // kBet: ベット額, kRaise: 上乗せ額
    public:
        explicit Action(ActionType type);
        Action(ActionType type, int size);
        [[nodiscard]] ActionType type() const;
        [[nodiscard]] int size() const;
    };
}

#endif //POKER_ACTION_H
