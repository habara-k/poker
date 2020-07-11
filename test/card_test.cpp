#include "gtest/gtest.h"

#include "card.h"

using namespace poker;


TEST(card, Card) {
    Card card(8);
    EXPECT_EQ(card.suit(), Suit::kS);
    EXPECT_EQ(card.rank(), Rank::k3);

    card = Card(Suit::kS, Rank::k3);
    EXPECT_EQ(card.suit(), Suit::kS);
    EXPECT_EQ(card.rank(), Rank::k3);
}

TEST(card, CardSet) {
    CardSet card_set;
    EXPECT_EQ(card_set.cards[8].suit(), Suit::kS);
    EXPECT_EQ(card_set.cards[8].rank(), Rank::k3);
}