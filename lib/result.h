#ifndef POKER_RESULT_H
#define POKER_RESULT_H

#include <map>

#include "stage.h"
#include "hand.h"

namespace poker {
    class Result {
        Stage stage_;   // kEndHidden or kShowdown
        int winner_;
        int pot_;
        std::optional<std::map<int,Hand>> hands_;
    public:
        Result(Stage stage, int winner, int pot);
        Result(Stage stage, int winner, int pot, std::map<int,Hand> hands);

        [[nodiscard]] Stage stage() const;
        [[nodiscard]] int winner() const;
        [[nodiscard]] int pot() const;
        [[nodiscard]] std::map<int,Hand> hands() const;
    };
}

#endif //POKER_RESULT_H
