#ifndef POKER_HAND_H
#define POKER_HAND_H

#include <array>
#include <optional>
#include <set>

#include "card.h"

namespace poker {
    enum HandCategory {
        kStraightFlush,
        kFourOfAKind,
        kFullHouse,
        kFlush,
        kStraight,
        kThreeOfAKind,
        kTwoPair,
        kOnePair,
        kHighCard
    };

    class Hand {
        HandCategory category_;
        std::array<Card,5> cards_;
    public:
        Hand(HandCategory category, const std::vector<Card>& hand);
        static Hand Create(
                const std::array<std::optional<Card>,2>& hole_cards,
                const std::array<Card,5>& community_cards);

        [[nodiscard]] HandCategory category() const;
        [[nodiscard]] const std::array<Card,5>& cards() const;

        struct RankCompare {
            bool operator() (const Hand& lhs, const Hand& rhs) const;
        };
    private:
        static std::optional<std::vector<Card>> HasStraight(const std::set<Card>& cards);
    };
}

#endif //POKER_HAND_H
