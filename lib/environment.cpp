#include "environment.h"

#include <cassert>
#include <string>
#include <memory>

#include "state.h"
#include "dealer.h"

namespace poker {
    Environment::Environment(const std::vector<std::string> &agent_types) {
        assert(2 <= agent_types.size() and agent_types.size() <= 9);
        for (const std::string& type : agent_types) {
            if (type == "check_call") {
                auto agent = std::make_unique<CheckCallAgent>();
                agents.emplace_back(std::move(agent));
            } else if (type == "check_fold") {
                    auto agent = std::make_unique<CheckFoldAgent>();
                    agents.emplace_back(std::move(agent));
            } else if (type == "user") {
                auto agent = std::make_unique<UserAgent>();
                agents.emplace_back(std::move(agent));
            } else {
                assert(false);
            }
        }
    }

    void Environment::Run() {
        int bb = 100, stack = 10000;
        auto [hole_cards, community_cards] = Dealer::Deal(agents.size());

        State state(bb, stack, hole_cards, community_cards);

        while (state.stage() != Stage::kShowdown and state.stage() != Stage::kEndHidden) {
            int next_id = state.next_player_id();
            Observable observable(state, next_id);
            auto possible_actions = state.PossibleActions();

            auto action = agents[state.next_player_id()]->ChooseAction(
                    observable, possible_actions);

            state.TakeAction(next_id, action);
        }

        for (auto& agent : agents) {
            agent->Reward(Observable(state));
        }
    }
}