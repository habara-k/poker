#include "agent.h"

#include <cassert>
#include <iostream>
#include <set>

#include "visualizer.h"

namespace poker {
    Agent::Agent() {}

    CheckCallAgent::CheckCallAgent() {}

    Action CheckCallAgent::ChooseAction(
            const Observable& observable,
            const std::vector<ActionRange>& possible_actions) {
        for (const auto& action_range : possible_actions) {
            if (action_range.type() == ActionType::kCall) {
                return Action(ActionType::kCall);
            }
            else if (action_range.type() == ActionType::kCheck) {
                return Action(ActionType::kCheck);
            }
        }
        assert(false);
    }

    UserAgent::UserAgent() {}

    Action UserAgent::ChooseAction(
            const Observable& observable,
            const std::vector<ActionRange>& possible_actions) {

        // Show Observable

        std::cout << "======================" << std::endl;
        std::cout << "stage: " << Visualizer::ToString(observable.stage()) << std::endl;
        std::cout << "pot: " << observable.pot() << std::endl;
        std::cout << "community cards:" << std::endl;
        for (const std::string& str : Visualizer::ToStrings(observable.community_cards())) {
            std::cout << str << std::endl;
        }

        std::cout << "you are player" << observable.player_id() << std::endl;
        const std::vector<Player>& players = observable.players();
        for (int i = 0; i < players.size(); ++i) {
            std::cout << std::endl;
            for (const std::string& str : Visualizer::ToStrings(players[i])) {
                std::cout << str << std::endl;
            }
        }
        std::cout << "======================" << std::endl;

        // Choose Action

        std::set<ActionType> action_types;
        for (const auto& action_range : possible_actions) {
            action_types.insert(action_range.type());
        }

        ActionType action_type;
        while (true) {
            std::cout << "アクションを選んでください" << std::endl;
            for (const ActionType type : action_types) {
                std::cout << static_cast<int>(type) << ':' << Visualizer::ToString(type) << ' ';
            }
            std::cout << "--> ";
            int type;
            std::cin >> type;
            if (action_types.find(static_cast<ActionType>(type)) != action_types.end()) {
                action_type = static_cast<ActionType>(type);
                break;
            } else {
                std::cout << "入力が正しくありません" << std::endl;
            }
        }

        if (action_type != ActionType::kBet and action_type != ActionType::kRaise) {
            return Action(action_type);
        }

        int min = -1, max = -1;
        for (const auto& action_range : possible_actions) {
            if (action_range.type() == action_type) {
                min = action_range.min();
                max = action_range.max();
            }
        }
        assert(min != -1);
        assert(max != -1);

        int size = -1;

        while (true) {
            std::cout << (action_type == ActionType::kBet ? "ベット" : "レイズ") <<
                    "額を入力してください" << std::endl;

            std::cout << "min:" << min << " max:" << max << " --> ";
            std::cin >> size;
            if (min <= size and size <= max) {
               break;
            } else {
                std::cout << "入力が正しくありません" << std::endl;
            }
        }

        return Action(action_type, size);
    }
}