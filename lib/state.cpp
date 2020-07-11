#include "state.h"

#include <cassert>

namespace poker {
    State::State(int bb, int stack, const std::vector<std::vector<Card>>& hall_cards)
            : pot_(0), stage_(Stage::kPreFlop), bb_(bb) {
        for (const auto& cards : hall_cards) {
            players_.emplace_back(stack, cards);
        }
        assert(players_.size() >= 2);
        players_[0].Bet(bb / 2);
        players_[1].Bet(bb);
        max_bet_ = bb;
        min_raise_size_ = bb;
        next_player_id_ = players_.size() == 2 ? 0 : 2;
    }

    void State::TakeAction(int player_id, const Action& action) {
        assert(remained_players() > 1);

        assert(player_id == next_player_id_);
        assert(!terminal_player_id_ or terminal_player_id_.value() != player_id);

        Player& player = players_[player_id];

        if (++next_player_id_ == players_.size()) next_player_id_ = 0;
        next_player_id_ = find_remained_player(next_player_id_);

        switch (action.type()) {
            case ActionType::kCheck:
                assert(max_bet_ == 0);
                break;
            case ActionType::kFold:
                assert(max_bet_ > 0);
                player.Fold();
                pot_ += player.Collected();
                break;
            case ActionType::kCall:
                assert(max_bet_ > 0);
                player.Bet(max_bet_ - player.bet());
                assert(player.bet() == max_bet_);
                if (!terminal_player_id_) {
                    terminal_player_id_ = player_id;
                }
                break;
            case ActionType::kBet:
                assert(max_bet_ == 0);
                assert(min_raise_size_ == bb_);
                assert(min_raise_size_ <= action.size());
                min_raise_size_ = action.size();
                player.Bet(action.size());
                assert(player.bet() == action.size());
                max_bet_ = player.bet();
                terminal_player_id_ = player_id;
                break;
            case ActionType::kRaise:
                assert(max_bet_ > 0);
                assert(min_raise_size_ <= action.size());
                min_raise_size_ = action.size();
                player.Bet(max_bet_ + action.size() - player.bet());
                assert(player.bet() == max_bet_ + action.size());
                max_bet_ = player.bet();
                terminal_player_id_ = player_id;
                break;
            case ActionType::kAllIn:
                player.Bet(player.stack());
                assert(player.stack() == 0);
                max_bet_ = player.bet();
                terminal_player_id_ = player_id;
        }
    }

    void State::Flop(const std::vector<Card>& cards) {
        ResetForNextStep();
        stage_ = Stage::kFlop;
        for (const Card& card : cards) {
            community_cards_.push_back(card);
        }
    }

    void State::Turn(const Card& card) {
        ResetForNextStep();
        stage_ = Stage::kTurn;
        community_cards_.push_back(card);
    }

    void State::River(const Card& card) {
        ResetForNextStep();
        stage_ = Stage::kRiver;
        community_cards_.push_back(card);
    }

    void State::EndHidden() {
        assert(remained_players() == 1);

        for (Player& player : players_) {
            if (player.folded()) continue;
            pot_ += player.Collected();
            player.Win(pot_);
            pot_ = 0;
        }
        stage_ = Stage::kEndHidden;
    }

    void State::Showdown() {
        // TODO
        assert(remained_players() > 1);

        stage_ = Stage::kShowdown;
    }

    const std::vector<Player>& State::players() const {
        return players_;
    }
    const std::vector<Card>& State::community_cards() const {
        return community_cards_;
    }
    int State::pot() const {
        return pot_;
    }
    Stage State::stage() const {
        return stage_;
    }

    int State::remained_players() const {
        int remained = 0;
        for (const Player& player : players_) {
            remained += !player.folded();
        }
        return remained;
    }

    int State::find_remained_player(int id) const {
        while (players_[id].folded()) {
            if (++id == players_.size()) id = 0;
        }
        return id;
    }

    void State::ResetForNextStep() {
        assert(remained_players() > 1);
        for (Player& player : players_) {
            if (player.folded()) continue;
            pot_ += player.Collected();
        }
        max_bet_ = 0;
        min_raise_size_ = bb_;
        next_player_id_ = players_.size() == 2 ? 1 : 0;
        next_player_id_ = find_remained_player(next_player_id_);
        terminal_player_id_ = std::nullopt;
    }
}
