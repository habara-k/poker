#include "observable.h"

namespace poker {
    Observable::Observable(const State& state, int player_id)
            : state_(state), players_(state.players()), player_id_(player_id) {
        for (int i = 0; i < players_.size(); ++i) {
            if (i == player_id) continue;
            players_[i].HideHoleCards();
        }
    }

    int Observable::player_id() const {
        return player_id_;
    }
    const std::vector<Player>& Observable::players() const {
        return players_;
    }
    std::array<std::optional<Card>,5> Observable::community_cards() const {
        return state_.community_cards();
    }
    int Observable::pot() const {
        return state_.pot();
    }
    Stage Observable::stage() const {
        return state_.stage();
    }
    const std::vector<ActionRecord>& Observable::trajectory() const {
        return state_.trajectory();
    }
}