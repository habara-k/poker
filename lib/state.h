#ifndef POKER_STATE_H
#define POKER_STATE_H

#include <array>
#include <vector>

#include "player.h"
#include "action.h"
#include "action_range.h"
#include "stage.h"

namespace poker {
    class State {
        std::vector<Player> players_;
        std::array<Card,5> community_cards_;
        int pot_;
        Stage stage_;
        int max_bet_;
        int min_raise_size_;
        int next_player_id_;
        std::optional<int> terminal_player_id_;
        const int bb_;

    public:
        explicit State(
                int bb, int stack,
                const std::vector<std::array<Card,2>>& hall_cards,
                const std::array<Card,5>& community_cards);

        [[nodiscard]] int next_player_id() const;
        [[nodiscard]] std::vector<ActionRange> PossibleActions() const;

        void TakeAction(int player_id, const Action& action);

        [[nodiscard]] const std::vector<Player>& players() const;
        [[nodiscard]] const std::array<Card,5>& community_cards() const;
        [[nodiscard]] int pot() const;
        [[nodiscard]] Stage stage() const;

    private:
        [[nodiscard]] int remained_players() const;
        [[nodiscard]] int find_remained_player(int id) const;
        void GoToTheNextStage();
        void EndHidden();
        void Showdown();
    };
}

#endif //POKER_STATE_H
