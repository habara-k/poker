#include "observable.h"

namespace poker {
    Observable::Observable(const State& state, int player_id, bool hide)
            : state_(state), players_(state.players()), player_id_(player_id) {
        if (!hide) return;
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
    CommunityCards Observable::community_cards() const {
        return state_.community_cards();
    }
    int Observable::pot() const {
        return state_.pot();
    }
    Stage Observable::stage() const {
        return state_.stage();
    }
    std::vector<ActionRecord>::const_iterator Observable::history() const {
        return state_.history(player_id_);
    }
    std::vector<ActionRecord>::const_iterator Observable::history_end() const {
        return state_.history_end();
    }
    const std::optional<Result>& Observable::result() const {
        return state_.result();
    }
}