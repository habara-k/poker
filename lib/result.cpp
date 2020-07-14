#include "result.h"

#include <cassert>
#include <utility>

namespace poker {
    Result::Result(Stage stage, std::map<int,int> winner_to_pot)
            : stage_(stage), winner_to_pot_(std::move(winner_to_pot)) {
        assert(stage == Stage::kEndHidden);
    }

    Result::Result(Stage stage, std::map<int,int> winner_to_pot, std::map<int,Hand> hands)
            : stage_(stage), winner_to_pot_(std::move(winner_to_pot)), hands_(hands) {
            assert(stage == Stage::kShowdown);
    }

    Stage Result::stage() const {
        return stage_;
    }

    const std::map<int,int>& Result::winner_to_pot() const {
        return winner_to_pot_;
    }

    std::map<int,Hand> Result::hands() const {
        assert(stage_ == Stage::kShowdown);
        return hands_.value();
    }
}