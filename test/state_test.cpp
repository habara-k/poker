#include "gtest/gtest.h"

#include "state.h"

using namespace poker;


TEST(state, HeadsUp) {

    std::vector<HoleCards> hole_cards{
            {Card(Suit::kH, Rank::kA),Card(Suit::kH, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)}
    };
    CommunityCards community_cards{
            Card(Suit::kH, Rank::kQ), Card(Suit::kC, Rank::k2), Card(Suit::kC, Rank::k5),
            Card(Suit::kH, Rank::k9), Card(Suit::kD, Rank::k4)
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, hole_cards, community_cards);
    EXPECT_EQ(state.players()[0].hole_cards(), hole_cards[0]);
    EXPECT_EQ(state.players()[1].hole_cards(), hole_cards[1]);
    EXPECT_EQ(state.players()[0].stack(), stack - bb / 2);
    EXPECT_EQ(state.players()[1].stack(), stack - bb);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);

    state.TakeAction(0, Action(ActionType::kRaise, 200));
    EXPECT_EQ(state.players()[0].stack(), stack - 300);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(1, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[1].stack(), stack - 300);

    EXPECT_EQ(state.stage(), Stage::kFlop);
    EXPECT_EQ(state.pot(), 300 * 2);

    state.TakeAction(1, Action(ActionType::kCheck));
    EXPECT_EQ(state.stage(), Stage::kFlop);
    state.TakeAction(0, Action(ActionType::kBet, 300));
    EXPECT_EQ(state.players()[0].stack(), stack - 600);
    EXPECT_EQ(state.stage(), Stage::kFlop);
    state.TakeAction(1, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[1].folded(), true);

    EXPECT_EQ(state.stage(), Stage::kEndHidden);
    EXPECT_EQ(state.players()[0].stack(), stack + 300 * 1);
}

TEST(state, HeadsUpLimpIn) {

    std::vector<HoleCards> hole_cards{
            {Card(Suit::kH, Rank::kA),Card(Suit::kH, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)}
    };
    CommunityCards community_cards{
            Card(Suit::kH, Rank::kQ), Card(Suit::kC, Rank::k2), Card(Suit::kC, Rank::k5),
            Card(Suit::kH, Rank::k9), Card(Suit::kD, Rank::k4)
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, hole_cards, community_cards);

    state.TakeAction(0, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[0].stack(), stack - 100);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(1, Action(ActionType::kCheck));
    EXPECT_EQ(state.players()[1].stack(), stack - 100);

    EXPECT_EQ(state.stage(), Stage::kFlop);
    EXPECT_EQ(state.pot(), 100 * 2);

    state.TakeAction(1, Action(ActionType::kCheck));
    EXPECT_EQ(state.stage(), Stage::kFlop);
    state.TakeAction(0, Action(ActionType::kBet, 200));
    EXPECT_EQ(state.players()[0].stack(), stack - 300);
    EXPECT_EQ(state.stage(), Stage::kFlop);
    state.TakeAction(1, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[1].folded(), true);

    EXPECT_EQ(state.stage(), Stage::kEndHidden);
    EXPECT_EQ(state.players()[0].stack(), stack + 100 * 1);
}


TEST(state, Multiway) {

    std::vector<HoleCards> cards{
            {Card(Suit::kH, Rank::kA),Card(Suit::kH, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)},
            {Card(Suit::kD, Rank::k7),Card(Suit::kS, Rank::k7)},
            {Card(Suit::kD, Rank::k6),Card(Suit::kS, Rank::k6)},
            {Card(Suit::kD, Rank::k5),Card(Suit::kS, Rank::k5)},
            {Card(Suit::kD, Rank::k4),Card(Suit::kS, Rank::k4)}
    };
    CommunityCards community_cards{
            Card(Suit::kH, Rank::kQ), Card(Suit::kC, Rank::k2), Card(Suit::kC, Rank::k5),
            Card(Suit::kH, Rank::k9), Card(Suit::kD, Rank::k4)
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, cards, community_cards);
    EXPECT_EQ(state.players()[0].hole_cards(), cards[0]);
    EXPECT_EQ(state.players()[1].hole_cards(), cards[1]);
    EXPECT_EQ(state.players()[2].hole_cards(), cards[2]);
    EXPECT_EQ(state.players()[3].hole_cards(), cards[3]);
    EXPECT_EQ(state.players()[4].hole_cards(), cards[4]);
    EXPECT_EQ(state.players()[5].hole_cards(), cards[5]);
    EXPECT_EQ(state.players()[0].stack(), stack - bb / 2);
    EXPECT_EQ(state.players()[1].stack(), stack - bb);
    EXPECT_EQ(state.players()[2].stack(), stack);
    EXPECT_EQ(state.players()[3].stack(), stack);
    EXPECT_EQ(state.players()[4].stack(), stack);
    EXPECT_EQ(state.players()[5].stack(), stack);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);

    state.TakeAction(2, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[2].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(3, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[3].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(4, Action(ActionType::kRaise, 200));
    EXPECT_EQ(state.players()[4].stack(), stack - 300);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(5, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[5].stack(), stack - 300);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(0, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[0].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(1, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[1].stack(), stack - 300);
    // CO が3BBにレイズ. BTNとBBがコール. SBはフォールド.

    EXPECT_EQ(state.stage(), Stage::kFlop);
    EXPECT_EQ(state.pot(), 300 * 3 + 50);

    state.TakeAction(1, Action(ActionType::kCheck));
    EXPECT_EQ(state.stage(), Stage::kFlop);
    state.TakeAction(4, Action(ActionType::kCheck));
    EXPECT_EQ(state.stage(), Stage::kFlop);
    state.TakeAction(5, Action(ActionType::kCheck));
    // チェックで回る.

    EXPECT_EQ(state.stage(), Stage::kTurn);
    EXPECT_EQ(state.pot(), 300 * 3 + 50);

    state.TakeAction(1, Action(ActionType::kBet, 500));
    EXPECT_EQ(state.players()[1].bet(), 500);
    EXPECT_EQ(state.players()[1].stack(), stack - 300 - 500);
    EXPECT_EQ(state.stage(), Stage::kTurn);
    state.TakeAction(4, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[4].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kTurn);
    state.TakeAction(5, Action(ActionType::kCall));
    EXPECT_EQ(state.players()[5].stack(), stack - 300 - 500);
    // BBが500点をベット. COが降りてBTNがコール.

    EXPECT_EQ(state.stage(), Stage::kRiver);
    EXPECT_EQ(state.pot(), 300 * 3 + 50 + 500 * 2);

    state.TakeAction(1, Action(ActionType::kCheck));
    EXPECT_EQ(state.stage(), Stage::kRiver);
    state.TakeAction(5, Action(ActionType::kBet, 800));
    EXPECT_EQ(state.stage(), Stage::kRiver);
    state.TakeAction(1, Action(ActionType::kAllIn));
    EXPECT_EQ(state.stage(), Stage::kRiver);
    state.TakeAction(5, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[5].folded(), true);
    // BBのチェックに対してBTNが800点をベット. これにBBがオールインしてBTNがフォールド.

    EXPECT_EQ(state.stage(), Stage::kEndHidden);
    EXPECT_EQ(state.players()[1].stack(), stack + 300 * 2 + 50 + 500 * 1 + 800);
}

TEST(state, AllFold) {

    std::vector<HoleCards> cards{
            {Card(Suit::kH, Rank::kA),Card(Suit::kH, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)},
            {Card(Suit::kD, Rank::k7),Card(Suit::kS, Rank::k7)},
            {Card(Suit::kD, Rank::k6),Card(Suit::kS, Rank::k6)},
            {Card(Suit::kD, Rank::k5),Card(Suit::kS, Rank::k5)},
            {Card(Suit::kD, Rank::k4),Card(Suit::kS, Rank::k4)}
    };
    CommunityCards community_cards{
            Card(Suit::kH, Rank::kQ), Card(Suit::kC, Rank::k2), Card(Suit::kC, Rank::k5),
            Card(Suit::kH, Rank::k9), Card(Suit::kD, Rank::k4)
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, cards, community_cards);
    EXPECT_EQ(state.players()[0].hole_cards(), cards[0]);
    EXPECT_EQ(state.players()[1].hole_cards(), cards[1]);
    EXPECT_EQ(state.players()[2].hole_cards(), cards[2]);
    EXPECT_EQ(state.players()[3].hole_cards(), cards[3]);
    EXPECT_EQ(state.players()[4].hole_cards(), cards[4]);
    EXPECT_EQ(state.players()[5].hole_cards(), cards[5]);
    EXPECT_EQ(state.players()[0].stack(), stack - bb / 2);
    EXPECT_EQ(state.players()[1].stack(), stack - bb);
    EXPECT_EQ(state.players()[2].stack(), stack);
    EXPECT_EQ(state.players()[3].stack(), stack);
    EXPECT_EQ(state.players()[4].stack(), stack);
    EXPECT_EQ(state.players()[5].stack(), stack);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);

    state.TakeAction(2, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[2].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(3, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[3].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(4, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[4].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(5, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[5].folded(), true);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(0, Action(ActionType::kFold));
    EXPECT_EQ(state.players()[0].folded(), true);
    // BB以外フォールド

    EXPECT_EQ(state.stage(), Stage::kEndHidden);
    EXPECT_EQ(state.players()[1].stack(), stack + 50);
}

TEST(state, HeadsChop) {

    std::vector<HoleCards> hole_cards{
            {Card(Suit::kH, Rank::k2),Card(Suit::kD, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)}
    };
    CommunityCards community_cards{
            Card(Suit::kH, Rank::kA), Card(Suit::kH, Rank::kK), Card(Suit::kH, Rank::kQ),
            Card(Suit::kH, Rank::kJ), Card(Suit::kH, Rank::kT)
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, hole_cards, community_cards);

    state.TakeAction(0, Action(ActionType::kAllIn));
    EXPECT_EQ(state.players()[0].stack(), 0);
    EXPECT_EQ(state.stage(), Stage::kPreFlop);
    state.TakeAction(1, Action(ActionType::kCall));

    EXPECT_EQ(state.stage(), Stage::kShowdown);
    EXPECT_EQ(state.players()[0].stack(), 10000);
    EXPECT_EQ(state.players()[1].stack(), 10000);
}

TEST(state, Chop) {

    std::vector<HoleCards> hole_cards{
            {Card(Suit::kH, Rank::k2),Card(Suit::kD, Rank::kK)},
            {Card(Suit::kD, Rank::k8),Card(Suit::kS, Rank::k8)},
            {Card(Suit::kD, Rank::k5),Card(Suit::kS, Rank::k4)},
            {Card(Suit::kD, Rank::k2),Card(Suit::kS, Rank::kJ)}
    };
    CommunityCards community_cards{
            Card(Suit::kH, Rank::kA), Card(Suit::kH, Rank::kK), Card(Suit::kH, Rank::kQ),
            Card(Suit::kH, Rank::kJ), Card(Suit::kH, Rank::kT)
    };

    int bb = 100;
    int stack = 10000;
    State state(bb, stack, hole_cards, community_cards);

    state.TakeAction(2, Action(ActionType::kAllIn));
    state.TakeAction(3, Action(ActionType::kCall));
    state.TakeAction(0, Action(ActionType::kFold));
    state.TakeAction(1, Action(ActionType::kCall));

    EXPECT_EQ(state.stage(), Stage::kShowdown);
    EXPECT_EQ(state.players()[0].stack(), stack - bb / 2);
    EXPECT_EQ(state.players()[1].stack(), stack + (50 / 3) + 1);
    EXPECT_EQ(state.players()[2].stack(), stack + (50 / 3) + 1);
    EXPECT_EQ(state.players()[3].stack(), stack + (50 / 3));
}
