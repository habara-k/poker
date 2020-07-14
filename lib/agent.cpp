#include "agent.h"

#include <cassert>
#include <iostream>
#include <set>

#include "visualizer.h"

namespace poker {
    Action CheckCallAgent::ChooseAction(
            const Observable& observable,
            const std::vector<ActionRange>& possible_actions) {
        for (const auto& action_range : possible_actions) {
            if (action_range.type() == ActionType::kCheck) {
                return Action(ActionType::kCheck);
            }
            if (action_range.type() == ActionType::kCall) {
                return Action(ActionType::kCall);
            }
        }
        assert(false);
    }

    void CheckCallAgent::Reward(const Observable& observable) {
    }

    Action CheckFoldAgent::ChooseAction(
            const Observable& observable,
            const std::vector<ActionRange>& possible_actions) {
        for (const auto& action_range : possible_actions) {
            if (action_range.type() == ActionType::kCheck) {
                return Action(ActionType::kCheck);
            }
            if (action_range.type() == ActionType::kFold) {
                return Action(ActionType::kFold);
            }
        }
        assert(false);
    }

    void CheckFoldAgent::Reward(const Observable& observable) {
    }

    Action UserAgent::ChooseAction(
            const Observable& observable,
            const std::vector<ActionRange>& possible_actions) {

        // Show Observable

        for (const std::string& str : Visualizer::ToStrings(observable)) {
            std::cout << str << std::endl;
        }

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

    void UserAgent::Reward(const Observable& observable) {
        for (const std::string& str : Visualizer::ToStrings(observable)) {
            std::cout << str << std::endl;
        }
    }
}