#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include <array>

#include "card.h"

namespace poker {
    class Player {
        int stack_;
        int bet_;
        bool folded_;
        std::array<Card,2> hall_cards_;
    public:
        Player(int stack, const std::array<Card,2>& cards);
        void Bet(int size);
        int Collected();
        void Fold();
        void Win(int pot);
        [[nodiscard]] int stack() const;
        [[nodiscard]] int bet() const;
        [[nodiscard]] bool folded() const;
        [[nodiscard]] const std::array<Card,2>& hall_cards() const;
    };
}

#endif //POKER_PLAYER_H
