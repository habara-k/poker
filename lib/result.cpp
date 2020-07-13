#include "result.h"

#include <cassert>

namespace poker {
    Result::Result() {}

    Result::Result(Stage stage, int winner, int pot)
            : stage_(stage), winner_(winner), pot_(pot) {
        assert(stage == Stage::kEndHidden);
    }

    Result::Result(Stage stage, int winner, int pot, std::map<int,Hand> hands)
            : stage_(stage), winner_(winner), pot_(pot), hands_(hands) {
        assert(stage == Stage::kShowdown);
    }

    Stage Result::stage() const {
        return stage_;
    }

    int Result::winner() const {
        return winner_;
    }

    int Result::pot() const {
        return pot_;
    }

    std::map<int,Hand> Result::hands() const {
        assert(stage_ == Stage::kShowdown);
        return hands_.value();
    }
}