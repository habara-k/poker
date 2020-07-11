#include "action.h"

#include <cassert>

namespace poker {
    Action::Action(ActionType type) : type_(type) {
        assert(type == ActionType::kCheck or type == ActionType::kFold or
               type == ActionType::kCall or type == ActionType::kAllIn);
    }

    Action::Action(ActionType type, int size) : type_(type), size_(size) {
        assert(type == ActionType::kBet or type == ActionType::kRaise);
    }

    ActionType Action::type() const {
        return type_;
    }

    int Action::size() const {
        assert(type_ == ActionType::kBet or type_ == ActionType::kRaise);
        return size_.value();
    }
}
