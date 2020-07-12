#include "card.h"

#include <iostream>
#include <numeric>

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

    bool Card::operator<(const Card& other) const {
        return id_ < other.id_;
    }

    bool Card::RankCompare::operator() (const Card& lhs, const Card& rhs) const {
        return lhs.rank() < rhs.rank();
    }

    CardSet::CardSet() {
        std::cerr << "CardSet::CardSet" << std::endl;
        cards.reserve(52);
        for (int i = 0; i < 52; ++i) {
            cards.push_back(static_cast<Card>(i));
        }
    }
}
