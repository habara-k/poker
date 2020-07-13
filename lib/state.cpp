#include "state.h"

#include <cassert>
#include <algorithm>
#include <iostream>
#include <map>

#include "card.h"
#include "hand.h"
#include "visualizer.h"

namespace poker {
    State::State(int bb, int stack,
                const std::vector<std::array<std::optional<Card>,2>>& hole_cards,
                const std::array<Card,5>& all_community_cards)
                : all_community_cards_(all_community_cards), pot_(0),
                  stage_(Stage::kPreFlop), someone_all_in_(false),
                  bookmark_(hole_cards.size()), bb_(bb) {

        for (int id = 0; id < hole_cards.size(); ++id) {
            players_.emplace_back(id, stack, hole_cards[id]);
        }
        assert(players_.size() >= 2);
        players_[0].Bet(bb / 2);
        players_[1].Bet(bb);
        max_bet_ = bb;
        min_raise_size_ = bb;
        next_player_id_ = players_.size() == 2 ? 0 : 2;

        Show();
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

        std::clog << "player" << player_id << " " << Visualizer::ToString(action) << std::endl;

        bookmark_[player_id] = trajectory_.size();
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
                someone_all_in_ = true;
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
    std::array<std::optional<Card>,5> State::community_cards() const {
        switch (stage_) {
            case Stage::kPreFlop:
                return {
                        std::nullopt,
                        std::nullopt,
                        std::nullopt,
                        std::nullopt,
                        std::nullopt};
            case Stage::kFlop:
                return {
                        std::make_optional(all_community_cards_[0]),
                        std::make_optional(all_community_cards_[1]),
                        std::make_optional(all_community_cards_[2]),
                        std::nullopt,
                        std::nullopt};
            case Stage::kTurn:
                return {
                        std::make_optional(all_community_cards_[0]),
                        std::make_optional(all_community_cards_[1]),
                        std::make_optional(all_community_cards_[2]),
                        std::make_optional(all_community_cards_[3]),
                        std::nullopt};
            case Stage::kRiver:
                return {
                        std::make_optional(all_community_cards_[0]),
                        std::make_optional(all_community_cards_[1]),
                        std::make_optional(all_community_cards_[2]),
                        std::make_optional(all_community_cards_[3]),
                        std::make_optional(all_community_cards_[4])};
            case Stage::kShowdown:
                return {
                        std::make_optional(all_community_cards_[0]),
                        std::make_optional(all_community_cards_[1]),
                        std::make_optional(all_community_cards_[2]),
                        std::make_optional(all_community_cards_[3]),
                        std::make_optional(all_community_cards_[4])};
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
    //const std::vector<Record>& State::trajectory() const {
    //    return trajectory_;
    //}
    std::vector<Record>::const_iterator State::trajectory(int player_id) const {
        return trajectory_.begin() + bookmark_[player_id];
    }
    std::vector<Record>::const_iterator State::trajectory_end() const {
        return trajectory_.end();
    }
    const Result& State::result() const {
        return result_;
    }

    void State::EndHidden() {
        assert(remained_players() == 1);

        stage_ = Stage::kEndHidden;
        trajectory_.emplace_back(stage_);

        // TODO: need?
        // Show();

        for (int i = 0; i < players_.size(); ++i) {
            Player& player = players_[i];
            if (player.folded()) continue;
            // player i is winner
            result_ = Result(stage_, i, pot_);

            // 勝者のpotが未回収なので, ここで回収する.
            pot_ += player.Collected();

            player.Win(pot_);
        }

        std::clog << "========Result========" << std::endl;
        for (const std::string& str : Visualizer::ToStrings(result_)) {
            std::clog << str << std::endl;
        }
        std::clog << "======================" << std::endl;
    }

    void State::Showdown() {
        assert(remained_players() > 1);

        stage_ = Stage::kShowdown;
        trajectory_.emplace_back(stage_);

        Show();

        // TODO: chop
        std::vector<int> player_ids;
        for (int i = 0; i < players_.size(); ++i) {
            if (players_[i].folded()) continue;
            player_ids.push_back(i);
        }

        std::map<int,Hand> hands;
        for (int id : player_ids) {
            hands.insert({id, Hand::Create(players_[id].hole_cards(), all_community_cards_)});
        }

        int winner = *std::min_element(player_ids.begin(), player_ids.end(), [&](int id0, int id1){
            return Hand::RankCompare()(hands.at(id0), hands.at(id1));
        });

        result_ = Result(stage_, winner, pot_, hands);

        players_[winner].Win(pot_);
        pot_ = 0;

        std::clog << "========Result========" << std::endl;
        for (const std::string& str : Visualizer::ToStrings(result_)) {
            std::clog << str << std::endl;
        }
        std::clog << "======================" << std::endl;
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

        if (someone_all_in_) {
            Showdown();
            return;
        }

        switch (stage_) {
            case Stage::kPreFlop:
                stage_ = Stage::kFlop;
                trajectory_.emplace_back(stage_);
                Show();
                break;
            case Stage::kFlop:
                stage_ = Stage::kTurn;
                trajectory_.emplace_back(stage_);
                Show();
                break;
            case Stage::kTurn:
                stage_ = Stage::kRiver;
                trajectory_.emplace_back(stage_);
                Show();
                break;
            case Stage::kRiver:
                Showdown();
                break;
            default:
                assert(false);
        }
    }

    void State::Show() const {
        std::clog << "======================" << std::endl;
        std::clog << "stage: " << Visualizer::ToString(stage_) << std::endl;
        std::clog << "pot: " << pot_ << std::endl;
        std::clog << "community cards:" << std::endl;
        for (const std::string& str : Visualizer::ToStrings(community_cards())) {
            std::clog << str << std::endl;
        }
        for (const std::string& str : Visualizer::ToStrings(players_)) {
            std::clog << str << std::endl;
        }
        std::clog << "======================" << std::endl;
    }
}
