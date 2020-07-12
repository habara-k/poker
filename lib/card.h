#ifndef POKER_CARD_H
#define POKER_CARD_H

#include <vector>

namespace poker {
    enum class Suit {
        kS, kH, kD, kC
    };

    enum class Rank {
        kA, kK, kQ, kJ, kT, k9,
        k8, k7, k6, k5, k4, k3, k2
    };

    class Card {
        unsigned id_;
    public:
        explicit Card(unsigned id);
        explicit Card(Suit suit, Rank rank);
        [[nodiscard]] Suit suit() const;
        [[nodiscard]] Rank rank() const;
        bool operator==(const Card& other) const;
        bool operator<(const Card& other) const;

        struct RankCompare {
            bool operator() (const Card& lhs, const Card& rhs) const;
        };
    };

    struct CardSet {
        std::vector<Card> cards;
        CardSet();
    };
}

#endif //POKER_CARD_H
