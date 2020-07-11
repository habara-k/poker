#include "player.h"

#include <cassert>
#include <utility>

namespace poker {
    Player::Player(int stack, const std::array<Card,2>& cards)
            : stack_(stack), bet_(0), folded_(false), hall_cards_(cards) {}

    void Player::Bet(int size) {
        assert(stack_ >= size);
        bet_ += size;
        stack_ -= size;
    }

    int Player::Collected() {
        int ret = bet_;
        bet_ = 0;
        return ret;
    }

    void Player::Fold() {
        folded_ = true;
    }

    void Player::Win(int pot) {
        stack_ += pot;
    }

    int Player::stack() const {
        return stack_;
    }

    int Player::bet() const {
        return bet_;
    }

    bool Player::folded() const {
        return folded_;
    }

    const std::array<Card,2>& Player::hall_cards() const {
        return hall_cards_;
    }
}