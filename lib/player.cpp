#include "player.h"

#include <cassert>
#include <utility>

namespace poker {
    Position create_position(int id, int player_num) {
        assert(2 <= player_num and player_num <= 9);
        assert(0 <= id and id < player_num);
        if (id == 0) return Position::kSB;
        if (id == 1) return Position::kBB;
        if (id == player_num - 1) return Position::kBTN;
        return Position::kOther;
    }

    Player::Player(int id, int stack, const HoleCards& cards, int player_num)
            : id_(id), stack_(stack), bet_(0), folded_(false),
              hole_cards_(cards), position_(create_position(id, player_num)) {}

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

    Position Player::position() const {
        return position_;
    }
}