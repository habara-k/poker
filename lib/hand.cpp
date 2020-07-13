#include "hand.h"

#include <cassert>
#include <map>
#include <set>

#include "card.h"

namespace poker {
    Hand::Hand(HandCategory category, const std::vector<Card>& hand)
            : category_(category), cards_{hand[0], hand[1], hand[2], hand[3], hand[4]} {
        assert(hand.size() == 5);
    }

    Hand Hand::Create(const HoleCards& hole_cards, const CommunityCards& community_cards) {
        std::set<Card> cards;
        for (const std::optional<Card>& card : hole_cards) {
            cards.insert(card.value());
        }
        for (const std::optional<Card>& card : community_cards) {
            cards.insert(card.value());
        }

        std::map<Suit, std::set<Card>> suit_to_cards;
        std::map<Rank, std::set<Card>> rank_to_cards;
        for (const Card &card : cards) {
            suit_to_cards[card.suit()].insert(card);
            rank_to_cards[card.rank()].insert(card);
        }

        // StraightFlush
        for (const auto& [_, st] : suit_to_cards) {
            if (st.size() < 5) continue;

            if (auto straight_flush = HasStraight(st); straight_flush) {
                return {HandCategory::kStraightFlush, straight_flush.value()};
            }
        }

        std::map<int, std::vector<Rank>> size_to_ranks;
        for (const auto&[rank, st] : rank_to_cards) {
            if (st.size() == 1) continue;
            size_to_ranks[st.size()].push_back(rank);
        }

        // FourOfAKind
        if (size_to_ranks.count(4)) {
            assert(!size_to_ranks[4].empty());
            const auto &four = rank_to_cards[size_to_ranks[4][0]];
            std::vector<Card> hand;
            for (const Card &card : four) {
                hand.push_back(card);
            }
            for (const Card &card : cards) {
                if (four.find(card) != four.end()) continue;
                hand.push_back(card);
                if (hand.size() == 5) break;
            }
            assert(hand.size() == 5);
            return {HandCategory::kFourOfAKind, hand};
        }

        // FullHouse
        if (size_to_ranks.count(3) and size_to_ranks.count(2)) {
            assert(!size_to_ranks[3].empty() and !size_to_ranks[2].empty());
            const auto &three = rank_to_cards[size_to_ranks[3][0]],
                    pair = rank_to_cards[size_to_ranks[2][0]];
            std::vector<Card> hand;
            for (const Card &card : three) {
                hand.push_back(card);
            }
            for (const Card &card : pair) {
                hand.push_back(card);
            }
            assert(hand.size() == 5);
            return {HandCategory::kFullHouse, hand};
        }

        // Flush
        for (const auto& [_, st] : suit_to_cards) {
            if (st.size() < 5) continue;

            std::vector<Card> hand;
            for (const auto &card : st) {
                hand.push_back(card);
                if (hand.size() == 5) break;
            }
            assert(hand.size() == 5);
            return {HandCategory::kFlush, hand};
        }

        // Straight
        if (auto straight = HasStraight(cards); straight) {
            return {HandCategory::kStraight, straight.value()};
        }

        // ThreeOfAKind
        if (size_to_ranks.count(3)) {
            assert(!size_to_ranks[3].empty());
            const auto &three = rank_to_cards[size_to_ranks[3][0]];
            std::vector<Card> hand;
            for (const Card &card : three) {
                hand.push_back(card);
            }
            for (const Card &card : cards) {
                if (three.find(card) != three.end()) continue;
                hand.push_back(card);
                if (hand.size() == 5) break;
            }
            assert(hand.size() == 5);
            return {HandCategory::kThreeOfAKind, hand};
        }

        // TwoPair
        if (size_to_ranks.count(2) and size_to_ranks[2].size() >= 2) {
            const auto &pair0 = rank_to_cards[size_to_ranks[2][0]],
                       &pair1 = rank_to_cards[size_to_ranks[2][1]];
            std::vector<Card> hand;
            for (const Card &card : pair0) {
                hand.push_back(card);
            }
            for (const Card &card : pair1) {
                hand.push_back(card);
            }
            for (const Card &card : cards) {
                if (pair0.find(card) != pair0.end() and pair1.find(card) != pair1.end()) continue;
                hand.push_back(card);
                if (hand.size() == 5) break;
            }
            assert(hand.size() == 5);
            return {HandCategory::kTwoPair, hand};
        }

        // OnePair
        if (size_to_ranks.count(2)) {
            assert(!size_to_ranks[2].empty());
            const auto &pair = rank_to_cards[size_to_ranks[2][0]];
            std::vector<Card> hand;
            for (const Card &card : pair) {
                hand.push_back(card);
            }
            for (const Card &card : cards) {
                if (pair.find(card) != pair.end()) continue;
                hand.push_back(card);
                if (hand.size() == 5) break;
            }
            assert(hand.size() == 5);
            return {HandCategory::kOnePair, hand};
        }

        // HighCard
        std::vector<Card> hand;
        for (const Card& card : cards) {
            hand.push_back(card);
            if (hand.size() == 5) break;
        }
        return {HandCategory::kHighCard, hand};
    }

    HandCategory Hand::category() const {
        return category_;
    }

    const std::array<Card,5>& Hand::cards() const {
        return cards_;
    }

    bool Hand::RankCompare::operator() (const Hand& lhs, const Hand& rhs) const {
        assert(lhs.cards_.size() == 5);
        assert(rhs.cards_.size() == 5);
        auto compare = Card::RankCompare();
        if (lhs.category_ == rhs.category_) {
            for (int i = 0; i < 5; ++i) {
                if (compare(lhs.cards_[i], rhs.cards_[i])) return true;
                if (compare(rhs.cards_[i], lhs.cards_[i])) return false;
            }
            return false;
        }
        return lhs.category_ < rhs.category_;
    }

    std::optional<std::vector<Card>> Hand::HasStraight(const std::set<Card>& cards) {

        std::map<Rank, Card> rank_to_card;
        unsigned bit = 0;
        for (const Card& card : cards) {
            bit |= 1u << static_cast<unsigned>(card.rank());
            rank_to_card.insert({card.rank(), card});
        }

        std::vector<unsigned> bits{
                0b0000000011111,
                0b0000000111110,
                0b0000001111100,
                0b0000011111000,
                0b0000111110000,
                0b0001111100000,
                0b0011111000000,
                0b0111110000000,
                0b1111100000000,
                0b1111000000001
        };
        for (int s = 0; s < bits.size(); ++s) {
            if ((bit & bits[s]) != bits[s]) continue;
            std::vector<Card> hand;
            for (int i = s; i < s + 5; ++i) {
                if (i == 13) {
                    hand.push_back(rank_to_card.at(static_cast<Rank>(0)));
                } else {
                    hand.push_back(rank_to_card.at(static_cast<Rank>(i)));
                }
            }
            assert(hand.size() == 5);
            return hand;
        }
        return std::nullopt;
    }
}
