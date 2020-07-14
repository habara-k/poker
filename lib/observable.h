#ifndef POKER_OBSERVABLE_H
#define POKER_OBSERVABLE_H

#include <vector>
#include <array>

#include "card.h"
#include "state.h"
#include "types.h"

namespace poker {
    class Observable {
        const State& state_;
        std::vector<Player> players_;
        int player_id_;
    public:
        Observable(const State& state, int player_id, bool hide = true);

        [[nodiscard]] int player_id() const;
        [[nodiscard]] const std::vector<Player>& players() const;
        [[nodiscard]] CommunityCards community_cards() const;
        [[nodiscard]] int pot() const;
        [[nodiscard]] Stage stage() const;
        [[nodiscard]] TrajectoryIterator trajectory() const;
        [[nodiscard]] TrajectoryIterator trajectory_end() const;
        //[[nodiscard]] const Trajectory& trajectory() const;
        [[nodiscard]] const std::optional<Result>& result() const;
    };
}

#endif //POKER_OBSERVABLE_H
