#include "gtest/gtest.h"

#include <map>

#include "dealer.h"

using namespace poker;


TEST(dealer, Deal) {
    auto [hole_cards, community_cards] = Dealer::Deal(6);

    std::map<Card,int> card_count;
    for (const auto& cards : hole_cards) {
        ++card_count[cards[0].value()];
        ++card_count[cards[1].value()];
    }
    for (const auto& card : community_cards) {
        ++card_count[card];
    }

    EXPECT_EQ(card_count.size(), 6 * 2 + 5);

    auto [hole_cards2, community_cards2] = Dealer::Deal(6);
    std::map<Card,int> card_count2;
    for (const auto& cards : hole_cards2) {
        ++card_count[cards[0].value()];
        ++card_count[cards[1].value()];
    }
    for (const auto& card : community_cards2) {
        ++card_count[card];
    }

    EXPECT_NE(card_count, card_count2);
}
