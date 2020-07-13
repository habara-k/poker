#ifndef POKER_VISUALIZER_H
#define POKER_VISUALIZER_H

#include <string>
#include <array>

#include "card.h"
#include "stage.h"
#include "hand.h"
#include "action.h"
#include "record.h"
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

        template<unsigned long size>
        [[nodiscard]] static std::array<std::string,4> ToStrings(const std::array<Card,size>& cards) {
            std::array<std::string, 4> ret;
            for (const Card& card : cards) {
                std::array<std::string, 4> strs = ToStrings(card);
                for (int i = 0; i < 4; ++i) {
                    ret[i] += strs[i];
                }
            }
            return ret;
        }

        template<unsigned long size>
        [[nodiscard]] static std::array<std::string,4> ToStrings(const std::array<std::optional<Card>,size>& cards) {
            std::array<std::string, 4> ret;
            for (const std::optional<Card>& card : cards) {
                std::array<std::string, 4> strs = ToStrings(card);
                for (int i = 0; i < 4; ++i) {
                    ret[i] += strs[i];
                }
            }
            return ret;
        }

        [[nodiscard]] static std::string ToString(Stage stage);
        [[nodiscard]] static std::string ToString(HandCategory category);
        [[nodiscard]] static std::string ToString(ActionType action_type);
        [[nodiscard]] static std::string ToString(const Action& action);
        [[nodiscard]] static std::string ToString(const Record& record);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Player& player);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Player& player, bool is_you);
        [[nodiscard]] static std::vector<std::string> ToStrings(const std::vector<Player>& players);
        [[nodiscard]] static std::vector<std::string> ToStrings(const std::vector<Player>& players, int player_id);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Hand& hand);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Result& result);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Observable& observable);
    };
}

#endif //POKER_VISUALIZER_H
