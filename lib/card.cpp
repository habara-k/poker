#include "card.h"

namespace poker {
    Card::Card(unsigned id) : id_(id) {}

    Suit Card::suit() const {
        return static_cast<Suit>(id_ & 0b11u);
    }

    Rank Card::rank() const {
        return static_cast<Rank>(id_ >> 2u);
    }
}
