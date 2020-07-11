#ifndef POKER_ACTION_RANGE_H
#define POKER_ACTION_RANGE_H

#include <optional>

#include "action.h"

namespace poker {
    class ActionRange {
        ActionType type_;
        std::optional<int> min_, max_;

    public:
        explicit ActionRange(ActionType type);
        ActionRange(ActionType type, int min, int max);
        [[nodiscard]] ActionType type() const;
        [[nodiscard]] int min() const;
        [[nodiscard]] int max() const;

        [[nodiscard]] bool Valid(const Action& action) const;
    };
}

#endif //POKER_ACTION_RANGE_H
