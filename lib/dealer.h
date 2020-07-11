#ifndef POKER_DEALER_H
#define POKER_DEALER_H

#include <utility>
#include <array>
#include <vector>

#include "card.h"

namespace poker {
    class Dealer {
        Dealer() = delete;
    public:
        static std::pair<std::vector<std::array<Card,2>>, std::array<Card,5>>
        Deal(int player_num);
    };
}

#endif //POKER_DEALER_H
