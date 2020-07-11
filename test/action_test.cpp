#include "gtest/gtest.h"

#include "action.h"

using namespace poker;


TEST(action, Action) {
    Action action(ActionType::kCheck);
    EXPECT_EQ(action.type(), ActionType::kCheck);

    action = Action(ActionType::kFold);
    EXPECT_EQ(action.type(), ActionType::kFold);

    action = Action(ActionType::kCall);
    EXPECT_EQ(action.type(), ActionType::kCall);

    action = Action(ActionType::kBet, 100);
    EXPECT_EQ(action.type(), ActionType::kBet);
    EXPECT_EQ(action.size(), 100);

    action = Action(ActionType::kRaise, 300);
    EXPECT_EQ(action.type(), ActionType::kRaise);
    EXPECT_EQ(action.size(), 300);

    action = Action(ActionType::kAllIn);
    EXPECT_EQ(action.type(), ActionType::kAllIn);
}
