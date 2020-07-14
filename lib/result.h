#ifndef POKER_RESULT_H
#define POKER_RESULT_H

#include <map>

#include "stage.h"
#include "hand.h"

namespace poker {
    class Result {
        Stage stage_;   // kEndHidden or kShowdown
        std::map<int,int> winner_to_pot_;
        std::optional<std::map<int,Hand>> hands_;
    public:
        Result(Stage stage, std::map<int,int> winner_to_pot_);
        Result(Stage stage, std::map<int,int> winner_to_pot_, std::map<int,Hand> hands);

        [[nodiscard]] Stage stage() const;
        [[nodiscard]] const std::map<int,int>& winner_to_pot() const;
        [[nodiscard]] std::map<int,Hand> hands() const;
    };
}

#endif //POKER_RESULT_H
