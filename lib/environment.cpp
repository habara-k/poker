#include "environment.h"

#include "state.h"
#include "dealer.h"

namespace poker {
    Environment::Environment(int agent_num) : agents(agent_num) {}

    void Environment::Run() {
        int bb = 100, stack = 10000;
        auto [hall_cards, community_cards] = Dealer::Deal(agents.size());

        State state(bb, stack, hall_cards, community_cards);

        while (state.stage() != Stage::kShowdown and state.stage() != Stage::kEndHidden) {
            int next_id = state.next_player_id();
            Observable observable(
                    state.community_cards(),
                    state.players()[next_id].hall_cards(),
                    state.trajectory());
            auto possible_actions = state.PossibleActions();

            auto action = agents[state.next_player_id()].ChooseAction(
                    observable, possible_actions);

            state.TakeAction(next_id, action);
        }

    }
}