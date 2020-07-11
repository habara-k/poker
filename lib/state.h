#ifndef POKER_STATE_H
#define POKER_STATE_H

#include <vector>

#include "player.h"
#include "action.h"
#include "stage.h"

namespace poker {
    class State {
        std::vector<Player> players_;
        std::vector<Card> community_cards_;
        int pot_;
        Stage stage_;
        int max_bet_;
        int min_raise_size_;
        int next_player_id_;
        std::optional<int> terminal_player_id_;
        const int bb_;

    public:
        explicit State(int bb, int stack, const std::vector<std::vector<Card>>& hall_cards);

        void TakeAction(int player_id, const Action& action);
        void Flop(const std::vector<Card>& cards);
        void Turn(const Card& card);
        void River(const Card& card);
        void EndHidden();
        void Showdown();

        [[nodiscard]] const std::vector<Player>& players() const;
        [[nodiscard]] const std::vector<Card>& community_cards() const;
        [[nodiscard]] int pot() const;
        [[nodiscard]] Stage stage() const;

    private:
        [[nodiscard]] int remained_players() const;
        [[nodiscard]] int find_remained_player(int id) const;
        void ResetForNextStep();
    };
}

#endif //POKER_STATE_H
