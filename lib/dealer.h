#ifndef POKER_DEALER_H
#define POKER_DEALER_H

#include <utility>
#include <array>
#include <vector>

#include "card.h"
#include "types.h"

namespace poker {
    class Dealer {
        Dealer() = delete;
    public:
        //static std::pair<std::vector<std::array<std::optional<Card>,2>>, std::array<Card,5>>
        static std::pair<std::vector<HoleCards>, CommunityCards>
        Deal(int player_num);
    };
}

#endif //POKER_DEALER_H
