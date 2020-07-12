#include "gtest/gtest.h"

#include "hand.h"

using namespace poker;

TEST(hand_category, HandCategory) {
    EXPECT_LT(HandCategory::kStraightFlush, HandCategory::kFourOfAKind);
    EXPECT_LT(HandCategory::kFourOfAKind, HandCategory::kFullHouse);
    EXPECT_LT(HandCategory::kFullHouse, HandCategory::kFlush);
    EXPECT_LT(HandCategory::kFlush, HandCategory::kStraight);
    EXPECT_LT(HandCategory::kStraight, HandCategory::kThreeOfAKind);
    EXPECT_LT(HandCategory::kThreeOfAKind, HandCategory::kTwoPair);
    EXPECT_LT(HandCategory::kTwoPair, HandCategory::kOnePair);
    EXPECT_LT(HandCategory::kOnePair, HandCategory::kHighCard);
}

TEST(hand, StraightFlush) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kH, Rank::kJ)
              },{
                    Card(Suit::kH, Rank::kT),
                    Card(Suit::kC, Rank::k2),
                    Card(Suit::kH, Rank::k7),
                    Card(Suit::kD, Rank::k2),
                    Card(Suit::kH, Rank::k9)
               });
    EXPECT_EQ(hand.category(), HandCategory::kStraightFlush);
}

TEST(hand, Flush) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kH, Rank::kJ)
              },{
                      Card(Suit::kH, Rank::k2),
                      Card(Suit::kC, Rank::k2),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kH, Rank::k9)
              });
    EXPECT_EQ(hand.category(), HandCategory::kFlush);
}

TEST(hand, FlushVsFlush) {
    Hand hand1 = Hand::Create({
                                     Card(Suit::kH, Rank::k8),
                                     Card(Suit::kH, Rank::kJ)
                             },{
                                     Card(Suit::kH, Rank::k2),
                                     Card(Suit::kC, Rank::k2),
                                     Card(Suit::kH, Rank::k7),
                                     Card(Suit::kD, Rank::k2),
                                     Card(Suit::kH, Rank::k9)
                             });
    Hand hand2 = Hand::Create({
                                      Card(Suit::kH, Rank::k5),
                                      Card(Suit::kH, Rank::kA)
                              },{
                                      Card(Suit::kH, Rank::k2),
                                      Card(Suit::kC, Rank::k2),
                                      Card(Suit::kH, Rank::k7),
                                      Card(Suit::kD, Rank::k2),
                                      Card(Suit::kH, Rank::k9)
                              });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), false);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), true);
}

TEST(hand, Straight) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kH, Rank::kJ)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::k2),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kH, Rank::k9)
              });
    EXPECT_EQ(hand.category(), HandCategory::kStraight);
}

TEST(hand, StraightVsStraight) {
    Hand hand1 = Hand::Create({
                                     Card(Suit::kH, Rank::k8),
                                     Card(Suit::kH, Rank::k6)
                             },{
                                     Card(Suit::kD, Rank::k9),
                                     Card(Suit::kC, Rank::k3),
                                     Card(Suit::kH, Rank::k7),
                                     Card(Suit::kD, Rank::k5),
                                     Card(Suit::kH, Rank::kA)
                             });
    Hand hand2 = Hand::Create({
                                      Card(Suit::kH, Rank::k4),
                                      Card(Suit::kH, Rank::k2)
                              },{
                                    Card(Suit::kD, Rank::k9),
                                    Card(Suit::kC, Rank::k3),
                                    Card(Suit::kH, Rank::k7),
                                    Card(Suit::kD, Rank::k5),
                                    Card(Suit::kH, Rank::kA)
    });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), true);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), false);
}

TEST(hand, FourOfAKind) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kD, Rank::k8)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::k8),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kS, Rank::k8)
              });
    EXPECT_EQ(hand.category(), HandCategory::kFourOfAKind);
}

TEST(hand, FourOfAKindVSFourOfAKind) {
    Hand hand1 = Hand::Create({
                                     Card(Suit::kH, Rank::k8),
                                     Card(Suit::kD, Rank::k8)
                             },{
                                     Card(Suit::kD, Rank::kT),
                                     Card(Suit::kC, Rank::k8),
                                     Card(Suit::kH, Rank::kT),
                                     Card(Suit::kD, Rank::k2),
                                     Card(Suit::kS, Rank::k8)
                             });
    Hand hand2 = Hand::Create({
                                     Card(Suit::kS, Rank::kT),
                                     Card(Suit::kC, Rank::kT)
                             },{
                                     Card(Suit::kD, Rank::kT),
                                     Card(Suit::kC, Rank::k8),
                                     Card(Suit::kH, Rank::kT),
                                     Card(Suit::kD, Rank::k2),
                                     Card(Suit::kS, Rank::k8)
                             });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), false);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), true);
}

TEST(hand, FourOfAKindVSFourOfAKindKikker) {
    Hand hand1 = Hand::Create({
                                      Card(Suit::kH, Rank::kA),
                                      Card(Suit::kD, Rank::k3)
                              },{
                                      Card(Suit::kD, Rank::k8),
                                      Card(Suit::kC, Rank::k8),
                                      Card(Suit::kH, Rank::k8),
                                      Card(Suit::kD, Rank::k2),
                                      Card(Suit::kS, Rank::k8)
                              });
    Hand hand2 = Hand::Create({
                                      Card(Suit::kS, Rank::kT),
                                      Card(Suit::kC, Rank::kT)
                              },{
                                      Card(Suit::kD, Rank::k8),
                                      Card(Suit::kC, Rank::k8),
                                      Card(Suit::kH, Rank::k8),
                                      Card(Suit::kD, Rank::k2),
                                      Card(Suit::kS, Rank::k8)
                              });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), true);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), false);
}

TEST(hand, FourOfAKindVSFourOfAKindChop) {
    Hand hand1 = Hand::Create({
                                      Card(Suit::kH, Rank::kT),
                                      Card(Suit::kD, Rank::k3)
                              },{
                                      Card(Suit::kD, Rank::k8),
                                      Card(Suit::kC, Rank::k8),
                                      Card(Suit::kH, Rank::k8),
                                      Card(Suit::kD, Rank::k2),
                                      Card(Suit::kS, Rank::k8)
                              });
    Hand hand2 = Hand::Create({
                                      Card(Suit::kS, Rank::kT),
                                      Card(Suit::kC, Rank::kT)
                              },{
                                      Card(Suit::kD, Rank::k8),
                                      Card(Suit::kC, Rank::k8),
                                      Card(Suit::kH, Rank::k8),
                                      Card(Suit::kD, Rank::k2),
                                      Card(Suit::kS, Rank::k8)
                              });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), false);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), false);
}

TEST(hand, ThreeOfAKind) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kD, Rank::k8)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::k8),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kS, Rank::k9)
              });
    EXPECT_EQ(hand.category(), HandCategory::kThreeOfAKind);
}

TEST(hand, FullHouse) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kD, Rank::k8)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::k8),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kS, Rank::k2)
              });
    EXPECT_EQ(hand.category(), HandCategory::kFullHouse);
}

TEST(hand, FullHouseVsFullHouse) {
    Hand hand1 = Hand::Create({
                                     Card(Suit::kH, Rank::k8),
                                     Card(Suit::kD, Rank::k8)
                             },{
                                     Card(Suit::kD, Rank::kT),
                                     Card(Suit::kC, Rank::k8),
                                     Card(Suit::kH, Rank::k7),
                                     Card(Suit::kD, Rank::k2),
                                     Card(Suit::kS, Rank::k2)
                             });
    Hand hand2 = Hand::Create({
                                      Card(Suit::kH, Rank::kT),
                                      Card(Suit::kH, Rank::k2)
                              },{
                                      Card(Suit::kD, Rank::kT),
                                      Card(Suit::kC, Rank::k8),
                                      Card(Suit::kH, Rank::k7),
                                      Card(Suit::kD, Rank::k2),
                                      Card(Suit::kS, Rank::k2)
                              });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), true);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), false);
}

TEST(hand, TwoPair) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kD, Rank::k8)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::kJ),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kS, Rank::k2)
              });
    EXPECT_EQ(hand.category(), HandCategory::kTwoPair);
}

TEST(hand, TwoPairVsTwoPair) {
    Hand hand1 = Hand::Create({
                                     Card(Suit::kH, Rank::k8),
                                     Card(Suit::kD, Rank::k8)
                             },{
                                     Card(Suit::kD, Rank::kT),
                                     Card(Suit::kC, Rank::kJ),
                                     Card(Suit::kH, Rank::k7),
                                     Card(Suit::kD, Rank::k2),
                                     Card(Suit::kS, Rank::k2)
                             });
    Hand hand2 = Hand::Create({
                                     Card(Suit::kH, Rank::kT),
                                     Card(Suit::kD, Rank::kA)
                             },{
                                     Card(Suit::kD, Rank::kT),
                                     Card(Suit::kC, Rank::kJ),
                                     Card(Suit::kH, Rank::k7),
                                     Card(Suit::kD, Rank::k2),
                                     Card(Suit::kS, Rank::k2)
                             });
    EXPECT_EQ(Hand::RankCompare()(hand1, hand2), false);
    EXPECT_EQ(Hand::RankCompare()(hand2, hand1), true);
}

TEST(hand, OnePair) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kD, Rank::kA)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::kJ),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k2),
                      Card(Suit::kS, Rank::k2)
              });
    EXPECT_EQ(hand.category(), HandCategory::kOnePair);
}

TEST(hand, HighCard) {
    Hand hand = Hand::Create({
                      Card(Suit::kH, Rank::k8),
                      Card(Suit::kD, Rank::kA)
              },{
                      Card(Suit::kD, Rank::kT),
                      Card(Suit::kC, Rank::kJ),
                      Card(Suit::kH, Rank::k7),
                      Card(Suit::kD, Rank::k3),
                      Card(Suit::kS, Rank::k2)
              });
    EXPECT_EQ(hand.category(), HandCategory::kHighCard);
}
