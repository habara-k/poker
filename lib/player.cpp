#include "player.h"

#include <cassert>
#include <utility>

namespace poker {
    //Player::Player(int id, int stack, const std::array<Card,2>& cards)
    //        : id_(id), stack_(stack), bet_(0), folded_(false),
    //          hole_cards_{std::make_optional(cards[0]),
    //                      std::make_optional(cards[1])} {}
    Player::Player(int id, int stack, const HoleCards& cards)
            : id_(id), stack_(stack), bet_(0), folded_(false), hole_cards_(cards) {}

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

    void Player::HideHoleCards() {
        hole_cards_ = {std::nullopt, std::nullopt};
    }

    int Player::id() const {
        return id_;
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

    const std::array<std::optional<Card>,2>& Player::hole_cards() const {
        return hole_cards_;
    }
}