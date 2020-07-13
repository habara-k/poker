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
                  stage_(Stage::kPreFlop), someone_all_in_(false), bb_(bb) {

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

        std::cerr << "player" << player_id << " " << Visualizer::ToString(action) << std::endl;

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
    const std::vector<ActionRecord>& State::trajectory() const {
        return trajectory_;
    }


    void State::EndHidden() {
        assert(remained_players() == 1);

        for (Player& player : players_) {
            if (player.folded()) continue;
            pot_ += player.Collected();
        }

        stage_ = Stage::kEndHidden;

        std::cerr << "========Result========" << std::endl;
        for (int i = 0; i < players_.size(); ++i) {
            Player& player = players_[i];
            if (player.folded()) continue;
            std::cerr << "player" << i << " が" << pot_ << " を獲得" << std::endl;
            player.Win(pot_);
            pot_ = 0;
        }
        std::cerr << "======================" << std::endl;
    }

    void State::Showdown() {
        // TODO
        assert(remained_players() > 1);

        stage_ = Stage::kShowdown;

        Show();

        // TODO: chop
        std::vector<int> player_ids;
        for (int i = 0; i < players_.size(); ++i) {
            if (players_[i].folded()) continue;
            player_ids.push_back(i);
        }

        std::cerr << "========Result========" << std::endl;
        std::map<int,Hand> hands;
        for (int id : player_ids) {
            Hand hand = Hand::Create(players_[id].hole_cards(), all_community_cards_);
            std::cerr << "player" << id << ": " << Visualizer::ToString(hand.category()) << std::endl;
            for (const std::string& str : Visualizer::ToStrings(hand.cards())) {
                std::cerr << str << std::endl;
            }
            hands.insert({id, hand});
        }

        int winner = *std::min_element(player_ids.begin(), player_ids.end(), [&](int id0, int id1){
            return Hand::RankCompare()(hands.at(id0), hands.at(id1));
        });

        std::cerr << "player" << winner << " が" << pot_ << " を獲得" << std::endl;
        std::cerr << "======================" << std::endl;
        players_[winner].Win(pot_);
        pot_ = 0;
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
                Show();
                break;
            case Stage::kFlop:
                stage_ = Stage::kTurn;
                Show();
                break;
            case Stage::kTurn:
                stage_ = Stage::kRiver;
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
        std::cerr << "======================" << std::endl;
        std::cerr << "stage: " << Visualizer::ToString(stage_) << std::endl;
        std::cerr << "pot: " << pot_ << std::endl;
        std::cerr << "community cards:" << std::endl;
        for (const std::string& str : Visualizer::ToStrings(community_cards())) {
            std::cerr << str << std::endl;
        }
        std::cerr << std::endl;
        for (int i = 0; i < players_.size(); ++i) {
            std::cerr << std::endl;
            for (const std::string& str : Visualizer::ToStrings(players_[i])) {
                std::cerr << str << std::endl;
            }
        }
        std::cerr << "======================" << std::endl;
    }
}
