#include "action_range.h"

#include <cassert>

namespace poker {
    ActionRange::ActionRange(ActionType type) : type_(type) {
        assert(type == ActionType::kCheck or type == ActionType::kFold or
               type == ActionType::kCall or type == ActionType::kAllIn);
    }

    ActionRange::ActionRange(ActionType type, int min, int max)
            : type_(type), min_(min), max_(max) {
        assert(type == ActionType::kBet or type == ActionType::kRaise);
        assert(min <= max);
    }

    ActionType ActionRange::type() const {
        return type_;
    }

    int ActionRange::min() const {
        assert(type_ == ActionType::kBet or type_ == ActionType::kRaise);
        return min_.value();
    }

    int ActionRange::max() const {
        assert(type_ == ActionType::kBet or type_ == ActionType::kRaise);
        return max_.value();
    }

    bool ActionRange::Valid(const Action& action) const {
        if (action.type() != type_) return false;
        if (type_ == ActionType::kBet or type_ == ActionType::kRaise) {
            return min_ <= action.size() and action.size() <= max_;
        } else {
            return true;
        }
    }
}