#ifndef POKER_OBSERVABLE_H
#define POKER_OBSERVABLE_H

#include <vector>
#include <array>

#include "card.h"
#include "action_range.h"
#include "action_record.h"

namespace poker {
    class Observable {
        std::vector<Card> community_cards_;
        std::array<Card,2> my_hall_cards_;
        std::vector<ActionRecord> trajectory_;
    public:
        Observable(
                std::vector<Card> community_cards,
                const std::array<Card,2>& my_hall_cards,
                std::vector<ActionRecord> trajectory);
    };
}

#endif //POKER_OBSERVABLE_H
