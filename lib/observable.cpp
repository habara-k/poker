#include "observable.h"

namespace poker {
    Observable::Observable(const State &state)
            : state_(state), players_(state.players()) {}

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
    std::vector<Record>::const_iterator Observable::trajectory() const {
        return state_.trajectory(player_id_);
    }
    std::vector<Record>::const_iterator Observable::trajectory_end() const {
        return state_.trajectory_end();
    }
    const Result& Observable::result() const {
        return state_.result();
    }
}