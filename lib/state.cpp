#include "state.h"

#include <cassert>
#include <algorithm>

namespace poker {
    State::State(int bb, int stack,
                const std::vector<std::array<Card,2>>& hall_cards,
                const std::array<Card,5>& community_cards)
                : all_community_cards_(community_cards), pot_(0),
                  stage_(Stage::kPreFlop), bb_(bb) {
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

    int State::next_player_id() const {
        return next_player_id_;
    }

    std::vector<ActionRange> State::PossibleActions() const {
        const Player& player = players_[next_player_id_];
        std::vector<ActionRange> action_range;
        if (max_bet_ == player.bet()) {
            action_range.emplace_back(ActionType::kCheck);
        }
        if (max_bet_ > player.bet()) {
            action_range.emplace_back(ActionType::kFold);
            action_range.emplace_back(ActionType::kCall);
        }
        if (max_bet_ == 0) {
            action_range.emplace_back(ActionType::kBet, min_raise_size_, player.stack());
        }
        if (max_bet_ > 0) {
            action_range.emplace_back(ActionType::kRaise, min_raise_size_,
                    player.stack() - max_bet_ + player.bet());
        }
        action_range.emplace_back(ActionType::kAllIn);
        return action_range;
    }

    void State::TakeAction(int player_id, const Action& action) {
        auto possible_actions = PossibleActions();
        assert(std::any_of(possible_actions.begin(), possible_actions.end(),
                [&action](ActionRange& range){ return range.Valid(action); }));

        assert(remained_players() > 1);

        assert(player_id == next_player_id_);
        assert(!terminal_player_id_ or terminal_player_id_.value() != player_id);

        trajectory_.emplace_back(player_id, action);

        Player& player = players_[player_id];

        switch (action.type()) {
            case ActionType::kCheck:
                //assert(max_bet_ == 0);
                assert(max_bet_ == player.bet());
                if (!terminal_player_id_) {
                    terminal_player_id_ = player_id;
                }
                break;

            case ActionType::kFold:
                //assert(max_bet_ > 0);
                assert(max_bet_ > player.bet());
                player.Fold();
                pot_ += player.Collected();
                if (remained_players() == 1) {
                    EndHidden();
                    return;
                }
                break;

            case ActionType::kCall:
                //assert(max_bet_ > 0);
                assert(max_bet_ > player.bet());
                player.Bet(max_bet_ - player.bet());
                // assert(player.bet() == max_bet_);
                if (!terminal_player_id_) {
                    terminal_player_id_ = player_id;
                }
                break;

            case ActionType::kBet:
                assert(max_bet_ == 0);
                // assert(min_raise_size_ == bb_);
                assert(min_raise_size_ <= action.size());
                assert(action.size() <= player.stack());
                min_raise_size_ = action.size();
                player.Bet(action.size());
                // assert(player.bet() == action.size());
                max_bet_ = player.bet();
                terminal_player_id_ = player_id;
                break;

            case ActionType::kRaise:
                assert(max_bet_ > 0);
                assert(min_raise_size_ <= action.size());
                assert(max_bet_ + action.size() - player.bet() <= player.stack());
                min_raise_size_ = action.size();
                player.Bet(max_bet_ + action.size() - player.bet());
                // assert(player.bet() == max_bet_ + action.size());
                max_bet_ = player.bet();
                terminal_player_id_ = player_id;
                break;

            case ActionType::kAllIn:
                player.Bet(player.stack());
                assert(player.stack() == 0);
                max_bet_ = player.bet();
                terminal_player_id_ = player_id;
        }

        if (++next_player_id_ == players_.size()) next_player_id_ = 0;
        next_player_id_ = find_remained_player(next_player_id_);

        if (next_player_id_ == terminal_player_id_) {
            GoToTheNextStage();
        }
    }

    const std::vector<Player>& State::players() const {
        return players_;
    }
    std::vector<Card> State::community_cards() const {
        switch (stage_) {
            case Stage::kPreFlop:
                return {all_community_cards_.begin(), all_community_cards_.begin()};
            case Stage::kFlop:
                return {all_community_cards_.begin(), all_community_cards_.begin() + 3};
            case Stage::kTurn:
                return {all_community_cards_.begin(), all_community_cards_.begin() + 4};
            case Stage::kRiver:
                return {all_community_cards_.begin(), all_community_cards_.begin() + 5};
            default:
                assert(false);
        }
    }
    //const std::array<Card,5>& State::all_community_cards() const {
    //    return all_community_cards_;
    //}
    int State::pot() const {
        return pot_;
    }
    Stage State::stage() const {
        return stage_;
    }
    const std::vector<ActionRecord>& State::trajectory() const {
        return trajectory_;
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

    void State::GoToTheNextStage() {
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

        switch (stage_) {
            case Stage::kPreFlop:
                stage_ = Stage::kFlop;
                break;
            case Stage::kFlop:
                stage_ = Stage::kTurn;
                break;
            case Stage::kTurn:
                stage_ = Stage::kRiver;
                break;
            case Stage::kRiver:
                stage_ = Stage::kShowdown;
                Showdown();
                break;
            default:
                assert(false);
        }
    }
}
