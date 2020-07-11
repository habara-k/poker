#include "observable.h"

namespace poker {
    Observable::Observable(
            std::vector<Card> community_cards,
            const std::array<Card, 2>& my_hall_cards,
            std::vector<ActionRecord> trajectory)
            : community_cards_(std::move(community_cards)),
              my_hall_cards_(my_hall_cards),
              trajectory_(std::move(trajectory)) {}
}