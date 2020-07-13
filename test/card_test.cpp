#include "gtest/gtest.h"

#include "card.h"

using namespace poker;

TEST(rank, Rank) {
    EXPECT_LT(Rank::kA, Rank::kK);
    EXPECT_LT(Rank::kK, Rank::kQ);
    EXPECT_LT(Rank::kQ, Rank::kJ);
    EXPECT_LT(Rank::kJ, Rank::kT);
    EXPECT_LT(Rank::kT, Rank::k9);
    EXPECT_LT(Rank::k9, Rank::k8);
    EXPECT_LT(Rank::k8, Rank::k7);
    EXPECT_LT(Rank::k7, Rank::k6);
    EXPECT_LT(Rank::k6, Rank::k5);
    EXPECT_LT(Rank::k5, Rank::k4);
    EXPECT_LT(Rank::k4, Rank::k3);
    EXPECT_LT(Rank::k3, Rank::k2);
}

TEST(card, Card) {
    auto card = Card(Suit::kS, Rank::k3);
    EXPECT_EQ(card.suit(), Suit::kS);
    EXPECT_EQ(card.rank(), Rank::k3);
}

TEST(card, CardSet) {
    CardSet card_set;
    EXPECT_EQ(card_set.cards[0].rank(), Rank::kA);
    EXPECT_EQ(card_set.cards[1].rank(), Rank::kA);
    EXPECT_EQ(card_set.cards[2].rank(), Rank::kA);
    EXPECT_EQ(card_set.cards[3].rank(), Rank::kA);
}
