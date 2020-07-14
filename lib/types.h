#ifndef POKER_TYPES_H
#define POKER_TYPES_H

#include <array>
#include <utility>

#include "card.h"
#include "action_record.h"

namespace poker {
    using CommunityCards = std::array<std::optional<Card>,5>;
    using HoleCards = std::array<std::optional<Card>,2>;
    using HandCards = std::array<Card,5>;
    using History = std::vector<ActionRecord>;
    using HistoryIterator = std::vector<ActionRecord>::const_iterator;
}

#endif //POKER_TYPES_H
