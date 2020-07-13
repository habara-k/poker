#ifndef POKER_HAND_H
#define POKER_HAND_H

#include <array>
#include <optional>
#include <set>

#include "card.h"
#include "types.h"

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
        HandCards cards_;
    public:
        Hand(HandCategory category, const std::vector<Card>& hand);
        static Hand Create(
                const HoleCards& hole_cards,
                const CommunityCards& community_cards);

        [[nodiscard]] HandCategory category() const;
        [[nodiscard]] const HandCards& cards() const;

        struct RankCompare {
            bool operator() (const Hand& lhs, const Hand& rhs) const;
        };
    private:
        static std::optional<std::vector<Card>> HasStraight(const std::set<Card>& cards);
    };
}

#endif //POKER_HAND_H
