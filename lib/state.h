#ifndef POKER_STATE_H
#define POKER_STATE_H

#include <array>
#include <vector>

#include "player.h"
#include "action.h"
#include "action_range.h"
#include "action_record.h"
#include "stage.h"

namespace poker {
    class State {
        std::vector<Player> players_;
        std::array<Card,5> all_community_cards_;
        int pot_;
        Stage stage_;
        int max_bet_;
        int min_raise_size_;
        int next_player_id_;
        bool someone_all_in_;
        std::optional<int> terminal_player_id_;
        std::vector<ActionRecord> trajectory_;
        const int bb_;

    public:
        explicit State(
                int bb, int stack,
                const std::vector<std::array<std::optional<Card>,2>>& hole_cards,
                const std::array<Card,5>& all_community_cards);

        [[nodiscard]] int next_player_id() const;
        [[nodiscard]] std::vector<ActionRange> PossibleActions() const;

        void TakeAction(int player_id, const Action& action);

        [[nodiscard]] const std::vector<Player>& players() const;
        [[nodiscard]] std::array<std::optional<Card>,5> community_cards() const;
        //[[nodiscard]]  std::array<Card,5>& all_community_cards() const;
        [[nodiscard]] int pot() const;
        [[nodiscard]] Stage stage() const;
        [[nodiscard]] const std::vector<ActionRecord>& trajectory() const;

    private:
        [[nodiscard]] int remained_players() const;
        [[nodiscard]] int find_remained_player(int id) const;
        void GoToTheNextStage();
        void EndHidden();
        void Showdown();
        void Show() const;
    };
}

#endif //POKER_STATE_H
