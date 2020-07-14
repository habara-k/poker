#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include <array>
#include <optional>

#include "card.h"
#include "types.h"

namespace poker {
    enum Position {
        kSB, kBB, kBTN, kOther
    };

    Position create_position(int id, int player_num);

    class Player {
        int id_;
        int stack_;
        int bet_;
        bool folded_;
        HoleCards hole_cards_;
        Position position_;
    public:
        Player(int id, int stack, const HoleCards& cards, int player_num);
        void Bet(int size);
        int Collected();
        void Fold();
        void Win(int pot);
        void HideHoleCards();
        [[nodiscard]] int id() const;
        [[nodiscard]] int stack() const;
        [[nodiscard]] int bet() const;
        [[nodiscard]] bool folded() const;
        [[nodiscard]] const HoleCards& hole_cards() const;
        [[nodiscard]] Position position() const;
    };
}

#endif //POKER_PLAYER_H
