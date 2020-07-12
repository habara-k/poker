#ifndef POKER_HAND_H
#define POKER_HAND_H

#include <array>
#include <optional>

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
        std::vector<Card> hand_;
    public:
        Hand(HandCategory category, std::vector<Card> hand);
        static Hand Create(const std::array<Card,2>& hall_cards, const std::array<Card,5>& community_cards);

        [[nodiscard]] HandCategory category() const;
        [[nodiscard]] const std::vector<Card>& hand() const;

        struct RankCompare {
            bool operator() (const Hand& lhs, const Hand& rhs) const;
        };
    private:
        template<typename S>
        static std::optional<std::vector<Card>> HasStraight(const S& cards);
    };
}

#endif //POKER_HAND_H
