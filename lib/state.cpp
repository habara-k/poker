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
                const std::vector<HoleCards>& hole_cards,
                const CommunityCards& all_community_cards)
                : all_community_cards_(all_community_cards), pot_(0),
                  stage_(Stage::kPreFlop), someone_all_in_(false),
                  bookmark_(hole_cards.size()), bb_(bb) {

        for (int id = 0; id < hole_cards.size(); ++id) {
            players_.emplace_back(id, stack, hole_cards[id], hole_cards.size());
        }
        assert(players_.size() >= 2);
        players_[0].Bet(bb / 2);
        players_[1].Bet(bb);
        max_bet_ = bb;
        min_raise_size_ = bb;
        next_player_id_ = players_.size() == 2 ? 0 : 2;
        trajectory_.emplace_back(stage_);

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
        if (max_bet_ == 0 and player.stack() > 0) {
            action_range.emplace_back(ActionType::kBet, min_raise_size_, player.stack());
        }
        if (int min = min_raise_size_,
                max = player.stack() - max_bet_ + player.bet();
                max_bet_ > 0 and min <= max) {
            action_range.emplace_back(ActionType::kRaise, min, max);
        }
        if (player.stack() - max_bet_ + player.bet() > 0) {
            action_range.emplace_back(ActionType::kAllIn);
        }
        return action_range;
    }

    void State::TakeAction(int player_id, Action action) {
        auto possible_actions = PossibleActions();
        assert(std::any_of(possible_actions.begin(), possible_actions.end(),
                [&action](ActionRange& range){ return range.Valid(action); }));

        assert(remained_players() > 1);

        assert(player_id == next_player_id_);
        assert(!terminal_player_id_ or terminal_player_id_.value() != player_id);

        Player& player = players_[player_id];

        // Raise max -> AllIn
        if (action.type() == ActionType::kRaise and max_bet_ + action.size() - player.bet() == player.stack()) {
            action = Action(ActionType::kAllIn);
        }

        std::clog << "player" << player_id << " " << Visualizer::ToString(action) << std::endl;

        bookmark_[player_id] = trajectory_.size();
        trajectory_.emplace_back(player_id, action);


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
    CommunityCards State::community_cards() const {
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
                        all_community_cards_[0],
                        all_community_cards_[1],
                        all_community_cards_[2],
                        std::nullopt,
                        std::nullopt};
            case Stage::kTurn:
                return {
                        all_community_cards_[0],
                        all_community_cards_[1],
                        all_community_cards_[2],
                        all_community_cards_[3],
                        std::nullopt};
            case Stage::kRiver:
                return all_community_cards_;
            case Stage::kShowdown:
                return all_community_cards_;
            case Stage::kEndHidden:
                return all_community_cards_;
        }
    }
    int State::pot() const {
        return pot_;
    }
    Stage State::stage() const {
        return stage_;
    }
    TrajectoryIterator State::trajectory(int player_id) const {
        return trajectory_.begin() + bookmark_[player_id];
    }
    TrajectoryIterator State::trajectory_end() const {
        return trajectory_.end();
    }
    const std::optional<Result>& State::result() const {
        return result_;
    }

    void State::EndHidden() {
        assert(remained_players() == 1);

        stage_ = Stage::kEndHidden;
        trajectory_.emplace_back(stage_);

        Show();

        for (int i = 0; i < players_.size(); ++i) {
            Player& player = players_[i];
            if (player.folded()) continue;
            // player i is winner
            result_ = Result(stage_, std::map<int,int>{{i, pot_}});

            // 勝者のpotが未回収なので, ここで回収する.
            pot_ += player.Collected();

            player.Win(pot_);
            pot_ = 0;
        }

        for (const std::string& str : Visualizer::ToStrings(result_.value())) {
            std::clog << str << std::endl;
        }
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

        // winnerを計算する.
        std::sort(player_ids.begin(), player_ids.end(), [&hands](int id0, int id1){
                return Hand::RankCompare()(hands.at(id0), hands.at(id1)); });

        auto end = std::upper_bound(player_ids.begin(), player_ids.end(), player_ids[0],
                [&hands](int id0, int id1){return Hand::RankCompare()(hands.at(id0), hands.at(id1)); });

        std::map<int, int> winner_to_pot;
        for (auto it = player_ids.begin(); it != end; ++it) {
            winner_to_pot[*it];
        }

        int quotient = pot_ / winner_to_pot.size(),
            reminder = pot_ % winner_to_pot.size();
        for (auto& [winner, pot] : winner_to_pot) {
            // player_id が小さい順に余りをもらう.
            // ヘッズアップはBBの方がSBより先にもらうべきだが, このときは2人なので余りが出ることはない.
            pot = quotient + (reminder ? --reminder, 1 : 0);
            players_[winner].Win(pot);
        }

        result_ = Result(stage_, winner_to_pot, hands);

        pot_ = 0;

        for (const std::string& str : Visualizer::ToStrings(result_.value())) {
            std::clog << str << std::endl;
        }
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
