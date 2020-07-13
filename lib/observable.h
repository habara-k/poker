#ifndef POKER_OBSERVABLE_H
#define POKER_OBSERVABLE_H

#include <vector>
#include <array>

#include "card.h"
#include "state.h"

namespace poker {
    class Observable {
        const State& state_;
        std::vector<Player> players_;
        int player_id_;
    public:
        Observable(const State& state, int player_id);

        [[nodiscard]] int player_id() const;
        [[nodiscard]] const std::vector<Player>& players() const;
        [[nodiscard]] std::array<std::optional<Card>,5> community_cards() const;
        [[nodiscard]] int pot() const;
        [[nodiscard]] Stage stage() const;
        [[nodiscard]] const std::vector<ActionRecord>& trajectory() const;
    };
}

#endif //POKER_OBSERVABLE_H
