#include "gtest/gtest.h"

#include "card.h"

using namespace poker;


TEST(card, Card) {
    Card card(8);
    EXPECT_EQ(card.suit(), Suit::kS);
    EXPECT_EQ(card.rank(), Rank::k3);
}