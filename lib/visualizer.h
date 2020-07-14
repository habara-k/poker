#ifndef POKER_VISUALIZER_H
#define POKER_VISUALIZER_H

#include <string>
#include <array>

#include "card.h"
#include "stage.h"
#include "hand.h"
#include "action.h"
#include "action_record.h"
#include "player.h"
#include "result.h"
#include "observable.h"

namespace poker {
    class Visualizer {
    public:
        [[nodiscard]] static std::string ToString(Suit suit);
        [[nodiscard]] static std::string ToString(Rank rank);
        [[nodiscard]] static std::array<std::string,4> ToStrings(const Card& card);
        [[nodiscard]] static std::array<std::string,4> ToStrings(const std::optional<Card>& card);

        [[nodiscard]] static std::string ToString(Stage stage);
        [[nodiscard]] static std::string ToString(HandCategory category);
        [[nodiscard]] static std::string ToString(ActionType action_type);
        [[nodiscard]] static std::string ToString(Position position);
        [[nodiscard]] static std::string ToString(const Action& action);
        [[nodiscard]] static std::string ToString(const ActionRecord& record);
        [[nodiscard]] static std::array<std::string,4> ToStrings(const HoleCards& cards);
        [[nodiscard]] static std::array<std::string,4> ToStrings(const HandCards& cards);
        [[nodiscard]] static std::array<std::string,4> ToStrings(const CommunityCards& cards);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Player& player);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Player& player, bool is_you);
        [[nodiscard]] static std::vector<std::string> ToStrings(const std::vector<Player>& players);
        [[nodiscard]] static std::vector<std::string> ToStrings(const std::vector<Player>& players, int your_id);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Hand& hand);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Result& result);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Observable& observable);
    };
}

#endif //POKER_VISUALIZER_H
