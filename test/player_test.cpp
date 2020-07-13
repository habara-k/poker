#include "gtest/gtest.h"

#include "player.h"

using namespace poker;


TEST(player, Player) {
    int id = 0;
    int stack = 10000;
    std::array<std::optional<Card>,2> cards{Card(19), Card(20)};
    Player player(id, stack, cards);
    EXPECT_EQ(player.id(), id);
    EXPECT_EQ(player.stack(), stack);
    EXPECT_EQ(player.bet(), 0);
    EXPECT_EQ(player.folded(), false);
    EXPECT_EQ(player.hole_cards(), cards);

    int bet = 200;
    player.Bet(bet);
    EXPECT_EQ(player.stack(), stack - bet);
    EXPECT_EQ(player.bet(), bet);

    EXPECT_EQ(player.Collected(), bet);
    EXPECT_EQ(player.stack(), stack - bet);
    EXPECT_EQ(player.bet(), 0);

    int pot = 1000;
    player.Win(pot);
    EXPECT_EQ(player.stack(), stack - bet + pot);

    player.Fold();
    EXPECT_EQ(player.folded(), true);
}
