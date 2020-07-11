#include "dealer.h"

#include <random>
#include <algorithm>

namespace poker {

    std::pair<std::vector<std::array<Card,2>>, std::array<Card,5>>
    Dealer::Deal(int player_num) {
        static CardSet card_set;
        static std::mt19937 engine;

        std::vector<Card> shuffled = card_set.cards;
        std::shuffle(shuffled.begin(), shuffled.end(), engine);

        std::array<Card,5> community_cards = {
                shuffled[0],
                shuffled[1],
                shuffled[2],
                shuffled[3],
                shuffled[4]};
        std::vector<std::array<Card,2>> hall_cards;
        hall_cards.reserve(player_num);
        for (int i = 0; i < player_num; ++i) {
            hall_cards.push_back({shuffled[5 + 2*i],
                                  shuffled[5 + 2*i + 1]});
        }
        return {hall_cards, community_cards};
    }
}