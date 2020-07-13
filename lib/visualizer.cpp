#include "visualizer.h"

namespace poker {
    std::string Visualizer::ToString(Suit suit) {
        switch (suit) {
            case Suit::kS:
                return "♠";
            case Suit::kH:
                return "♥";
            case Suit::kD:
                return "♦";
            case Suit::kC:
                return "♣";
        }
    }

    std::string Visualizer::ToString(Rank rank) {
        switch (rank) {
            case Rank::kA:
                return "A";
            case Rank::kK:
                return "K";
            case Rank::kQ:
                return "Q";
            case Rank::kJ:
                return "J";
            case Rank::kT:
                return "T";
            case Rank::k9:
                return "9";
            case Rank::k8:
                return "8";
            case Rank::k7:
                return "7";
            case Rank::k6:
                return "6";
            case Rank::k5:
                return "5";
            case Rank::k4:
                return "4";
            case Rank::k3:
                return "3";
            case Rank::k2:
                return "2";
        }
    }

    std::array<std::string,4> Visualizer::ToStrings(const Card& card) {
        return {
                "┌─────┐",
                "│ " + ToString(card.rank()) + "   │",
                "│   " + ToString(card.suit()) + " │",
                "└─────┘"
        };
    }

    std::array<std::string,4> Visualizer::ToStrings(const std::optional<Card>& card) {
        if (card) {
            return ToStrings(card.value());
        }
        return {
                "┌─────┐",
                "│░░░░░│",
                "│░░░░░│",
                "└─────┘"
        };
    }

    std::string Visualizer::ToString(Stage stage) {
        switch (stage) {
            case Stage::kPreFlop:
                return "PreFlop";
            case Stage::kFlop:
                return "Flop";
            case Stage::kTurn:
                return "Turn";
            case Stage::kRiver:
                return "kRiver";
            case Stage::kEndHidden:
                return "EndHidden";
            case Stage::kShowdown:
                return "Showdown";
        }
    }

    std::string Visualizer::ToString(HandCategory category) {
        switch (category) {
            case HandCategory::kStraightFlush:
                return "StraightFlush";
            case HandCategory::kFourOfAKind:
                return "FourOfAKind";
            case HandCategory::kFullHouse:
                return "FullHouse";
            case HandCategory::kFlush:
                return "Flush";
            case HandCategory::kStraight:
                return "Straight";
            case HandCategory::kThreeOfAKind:
                return "ThreeOfAKind";
            case HandCategory::kTwoPair:
                return "TwoPair";
            case HandCategory::kOnePair:
                return "OnePair";
            case HandCategory::kHighCard:
                return "HighCard";
        }
    }

    std::string Visualizer::ToString(ActionType action_type) {
        switch (action_type) {
            case ActionType::kCheck:
                return "Check";
            case ActionType::kFold:
                return "Fold";
            case ActionType::kCall:
                return "Call";
            case ActionType::kBet:
                return "Bet";
            case ActionType::kRaise:
                return "Raise";
            case ActionType::kAllIn:
                return "AllIn";
        }
    }

    std::string Visualizer::ToString(const Action& action) {
        ActionType type = action.type();
        if (type == ActionType::kBet or type == ActionType::kRaise) {
            return ToString(type) + ' ' + std::to_string(action.size());
        } else {
            return ToString(type);
        }
    }

    std::vector<std::string> Visualizer::ToStrings(const Player& player) {
        std::vector<std::string> ret;
        if (player.folded()) {
            return {"player" + std::to_string(player.id()) + " folded"};
        }
        ret.emplace_back("player" + std::to_string(player.id()));
        ret.emplace_back("  bet: " + std::to_string(player.bet()));
        ret.emplace_back("  stack: " + std::to_string(player.stack()));
        ret.emplace_back("  hole cards:");
        for (const std::string& str : Visualizer::ToStrings(player.hole_cards())) {
            ret.emplace_back(str);
        }
        return ret;
    }
}
