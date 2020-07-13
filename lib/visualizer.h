#ifndef POKER_VISUALIZER_H
#define POKER_VISUALIZER_H

#include <string>
#include <array>

#include "card.h"
#include "stage.h"
#include "hand.h"
#include "action.h"
#include "player.h"

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
                ret[0] += strs[0];
                ret[1] += strs[1];
                ret[2] += strs[2];
                ret[3] += strs[3];
            }
            return ret;
        }

        template<unsigned long size>
        [[nodiscard]] static std::array<std::string,4> ToStrings(const std::array<std::optional<Card>,size>& cards) {
            std::array<std::string, 4> ret;
            for (const std::optional<Card>& card : cards) {
                std::array<std::string, 4> strs = ToStrings(card);
                ret[0] += strs[0];
                ret[1] += strs[1];
                ret[2] += strs[2];
                ret[3] += strs[3];
            }
            return ret;
        }

        [[nodiscard]] static std::string ToString(Stage stage);
        [[nodiscard]] static std::string ToString(HandCategory category);
        [[nodiscard]] static std::string ToString(ActionType action_type);
        [[nodiscard]] static std::string ToString(const Action& action);
        [[nodiscard]] static std::vector<std::string> ToStrings(const Player& player);
    };
}

#endif //POKER_VISUALIZER_H
