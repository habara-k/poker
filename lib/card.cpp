#include "card.h"

namespace poker {
    Card::Card(unsigned id) : id_(id) {}

    Card::Card(Suit suit, Rank rank)
        : id_(static_cast<unsigned>(suit) | static_cast<unsigned>(rank) << 2u) {}

    Suit Card::suit() const {
        return static_cast<Suit>(id_ & 0b11u);
    }

    Rank Card::rank() const {
        return static_cast<Rank>(id_ >> 2u);
    }

    bool Card::operator==(const Card& other) const {
        return id_ == other.id_;
    }
}
