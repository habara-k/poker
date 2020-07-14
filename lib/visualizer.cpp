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

    std::string Visualizer::ToString(Position position) {
        switch (position) {
            case Position::kSB:
                return "(SB)";
            case Position::kBB:
                return "(BB)";
            case Position::kBTN:
                return "(BTN)";
            case Position::kOther:
                return "";
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

    std::string Visualizer::ToString(const ActionRecord& record) {
        return "- player" + std::to_string(record.player_id()) + ' ' + ToString(record.action());
    }

    std::array<std::string,4> Visualizer::ToStrings(const HoleCards& cards) {
        std::array<std::string, 4> ret;
        for (const std::optional<Card>& card : cards) {
            std::array<std::string, 4> strs = ToStrings(card);
            for (int i = 0; i < 4; ++i) {
                ret[i] += strs[i];
            }
        }
        return ret;
    }

    std::array<std::string,4> Visualizer::ToStrings(const HandCards& cards) {
        std::array<std::string, 4> ret;
        for (const Card& card : cards) {
            std::array<std::string, 4> strs = ToStrings(card);
            for (int i = 0; i < 4; ++i) {
                ret[i] += strs[i];
            }
        }
        return ret;
    }

    std::array<std::string,4> Visualizer::ToStrings(const CommunityCards& cards) {
        std::array<std::string, 4> ret;
        for (const std::optional<Card>& card : cards) {
            std::array<std::string, 4> strs = ToStrings(card);
            for (int i = 0; i < 4; ++i) {
                ret[i] += strs[i];
            }
        }
        return ret;
    }

    std::vector<std::string> Visualizer::ToStrings(const Player& player) {
        std::vector<std::string> ret;
        ret.emplace_back("player" + std::to_string(player.id()) + ' ' + ToString(player.position()));
        ret.back().resize(15, ' ');
        ret.emplace_back("---------------");
        if (player.folded()) {
            ret.emplace_back("folded         ");
            ret.emplace_back("               ");
            ret.emplace_back("               ");
            ret.emplace_back("               ");
            ret.emplace_back("               ");
            ret.emplace_back("               ");
            ret.emplace_back("               ");
        } else {
            ret.emplace_back("bet: " + std::to_string(player.bet()));
            ret.back().resize(15, ' ');
            ret.emplace_back("stack: " + std::to_string(player.stack()));
            ret.back().resize(15, ' ');
            ret.emplace_back("hole cards:");
            ret.back().resize(15, ' ');
            for (const std::string &str : ToStrings(player.hole_cards())) {
                ret.emplace_back(str + ' ');
            }
        }
        return ret;
    }

    std::vector<std::string> Visualizer::ToStrings(const Player& player, bool is_you) {
        std::vector<std::string> ret;
        if (is_you) {
            ret.emplace_back("▼ you          ");
        } else {
            ret.emplace_back("               ");
        }
        for (const std::string& str : ToStrings(player)) {
            ret.emplace_back(str);
        }
        return ret;
    }

    std::vector<std::string> Visualizer::ToStrings(const std::vector<Player>& players) {
        std::vector<std::string> ret(9);
        for (const Player player : players) {
            std::vector<std::string> strs = ToStrings(player);
            for (int i = 0; i < 9; ++i) {
                ret[i] += strs[i];
            }
        }
        return ret;
    }

    std::vector<std::string> Visualizer::ToStrings(const std::vector<Player>& players, int your_id) {
        std::vector<std::string> ret(10);
        for (int player_id = 0; player_id < players.size(); ++player_id) {
            std::vector<std::string> strs = ToStrings(players[player_id], player_id == your_id);
            for (int i = 0; i < 10; ++i) {
                ret[i] += strs[i];
            }
        }
        return ret;
    }

    std::vector<std::string> Visualizer::ToStrings(const Result& result) {
        std::vector<std::string> ret;
        ret.emplace_back("========Result========");
        if (result.stage() == Stage::kShowdown) {
            for (const auto& [id, hand] : result.hands()) {
                ret.emplace_back("player" + std::to_string(id) + ": " + ToString(hand.category()));
                for (const std::string& str : ToStrings(hand.cards())) {
                    ret.emplace_back(str);
                }
            }
        }
        for (const auto& [winner, pot] : result.winner_to_pot()) {
            ret.emplace_back(
                    "player" + std::to_string(winner) + " がポットの" +
                    std::to_string(pot) + " を獲得");
        }
        ret.emplace_back("======================");
        return ret;
    }

    std::vector<std::string> Visualizer::ToStrings(const Observable& observable) {
        std::vector<std::string> ret;
        ret.emplace_back("======================");
        ret.emplace_back("history:");
        for (auto it = observable.history(); it != observable.history_end(); ++it) {
            ret.emplace_back(ToString(*it));
        }
        ret.emplace_back("-----------------");
        ret.emplace_back("stage: " + ToString(observable.stage()));
        ret.emplace_back("pot: " + std::to_string(observable.pot()));
        ret.emplace_back("community cards:");
        for (const std::string& str : ToStrings(observable.community_cards())) {
            ret.emplace_back(str);
        }
        for (const std::string& str : ToStrings(observable.players(), observable.player_id())) {
            ret.emplace_back(str);
        }
        ret.emplace_back("======================");

        if (observable.result()) {
            for (const std::string& str : ToStrings(observable.result().value())) {
                ret.emplace_back(str);
            }
        }
        return ret;
    }
}
