#include "gtest/gtest.h"

#include "state.h"

using namespace poker;


TEST(state, HeadsUp) {

    std::vector<std::vector<Card>> cards{
            {Card(Suit::kH, Rank::kA),Card(Suit::kH, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)}
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, cards);
    EXPECT_EQ(state.players()[0].hall_cards(), cards[0]);
    EXPECT_EQ(state.players()[1].hall_cards(), cards[1]);
    EXPECT_EQ(state.players()[0].stack(), stack - bb / 2);
    EXPECT_EQ(state.players()[1].stack(), stack - bb);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);

    state.TakeAction(0, Action(ActionType::kRaise, 200));
    EXPECT_EQ(state.players()[0].stack(), stack - 300);
    state.TakeAction(1, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[1].stack(), stack - 300);

    std::vector<Card> flop{
                       Card(Suit::kS, Rank::kQ),
                       Card(Suit::kD, Rank::k2),
                       Card(Suit::kH, Rank::k8)};
    state.Flop(flop);
    EXPECT_EQ(state.community_cards(), flop);
    EXPECT_EQ(state.stage(), Stage::kFlop);
    EXPECT_EQ(state.pot(), 300 * 2);

    state.TakeAction(1, Action(ActionType::kCheck));
    state.TakeAction(0, Action(ActionType::kBet, 300));
    EXPECT_EQ(state.players()[0].stack(), stack - 600);
    state.TakeAction(1, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[1].folded(), true);

    state.EndHidden();
    EXPECT_EQ(state.stage(), Stage::kEndHidden);
    EXPECT_EQ(state.players()[0].stack(), stack + 300 * 1);
}

TEST(state, Multiway) {

    std::vector<std::vector<Card>> cards{
            {Card(Suit::kH, Rank::kA),Card(Suit::kH, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)},
            {Card(Suit::kD, Rank::k7),Card(Suit::kS, Rank::k7)},
            {Card(Suit::kD, Rank::k6),Card(Suit::kS, Rank::k6)},
            {Card(Suit::kD, Rank::k5),Card(Suit::kS, Rank::k5)},
            {Card(Suit::kD, Rank::k4),Card(Suit::kS, Rank::k4)}
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, cards);
    EXPECT_EQ(state.players()[0].hall_cards(), cards[0]);
    EXPECT_EQ(state.players()[1].hall_cards(), cards[1]);
    EXPECT_EQ(state.players()[2].hall_cards(), cards[2]);
    EXPECT_EQ(state.players()[3].hall_cards(), cards[3]);
    EXPECT_EQ(state.players()[4].hall_cards(), cards[4]);
    EXPECT_EQ(state.players()[5].hall_cards(), cards[5]);
    EXPECT_EQ(state.players()[0].stack(), stack - bb / 2);
    EXPECT_EQ(state.players()[1].stack(), stack - bb);
    EXPECT_EQ(state.players()[2].stack(), stack);
    EXPECT_EQ(state.players()[3].stack(), stack);
    EXPECT_EQ(state.players()[4].stack(), stack);
    EXPECT_EQ(state.players()[5].stack(), stack);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);

    state.TakeAction(2, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[2].folded(), true);
    state.TakeAction(3, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[3].folded(), true);
    state.TakeAction(4, Action(ActionType::kRaise, 200));
    EXPECT_EQ(state.players()[4].stack(), stack - 300);
    state.TakeAction(5, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[5].stack(), stack - 300);
    state.TakeAction(0, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[0].folded(), true);
    state.TakeAction(1, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[1].stack(), stack - 300);
    // CO が3BBにレイズ. BTNとBBがコール. SBはフォールド.

    std::vector<Card> flop{
            Card(Suit::kS, Rank::kQ),
            Card(Suit::kD, Rank::k2),
            Card(Suit::kH, Rank::k8)};
    state.Flop(flop);
    EXPECT_EQ(state.community_cards(), flop);
    EXPECT_EQ(state.stage(), Stage::kFlop);
    EXPECT_EQ(state.pot(), 300 * 3 + 50);

    state.TakeAction(1, Action(ActionType::kCheck));
    state.TakeAction(4, Action(ActionType::kCheck));
    state.TakeAction(5, Action(ActionType::kCheck));
    // チェックで回る.

    Card turn(Suit::kD, Rank::kQ);
    state.Turn(turn);
    EXPECT_EQ(state.community_cards()[3], turn);
    EXPECT_EQ(state.stage(), Stage::kTurn);
    EXPECT_EQ(state.pot(), 300 * 3 + 50);

    state.TakeAction(1, Action(ActionType::kBet, 500));
    EXPECT_EQ(state.players()[1].bet(), 500);
    EXPECT_EQ(state.players()[1].stack(), stack - 300 - 500);
    state.TakeAction(4, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[4].folded(), true);
    state.TakeAction(5, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[5].bet(), 500);
    EXPECT_EQ(state.players()[5].stack(), stack - 300 - 500);
    // BBが500点をベット. COが降りてBTNがコール.

    Card river(Suit::kC, Rank::kA);
    state.River(river);
    EXPECT_EQ(state.community_cards()[4], river);
    EXPECT_EQ(state.stage(), Stage::kRiver);
    EXPECT_EQ(state.pot(), 300 * 3 + 50 + 500 * 2);

    state.TakeAction(1, Action(ActionType::kCheck));
    state.TakeAction(5, Action(ActionType::kBet, 800));
    state.TakeAction(1, Action(ActionType::kAllIn));
    state.TakeAction(5, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[5].folded(), true);
    // BBのチェックに対してBTNが800点をベット. これにBBがオールインしてBTNがフォールド.

    state.EndHidden();
    EXPECT_EQ(state.players()[1].stack(), stack + 300 * 2 + 50 + 500 * 1 + 800);
}
