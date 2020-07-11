#ifndef POKER_CARD_H
#define POKER_CARD_H

namespace poker {
    enum class Suit {
        kS, kH, kD, kC
    };

    enum class Rank {
        kA, k2, k3, k4, k5, k6, k7,
        k8, k9, kT, kJ, kQ, kK
    };

    class Card {
        unsigned id_;
    public:
        explicit Card(unsigned id);
        explicit Card(Suit suit, Rank rank);
        [[nodiscard]] Suit suit() const;
        [[nodiscard]] Rank rank() const;
        bool operator==(const Card& other) const;
    };
}

#endif //POKER_CARD_H
