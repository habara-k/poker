#ifndef POKER_ENVIRONMENT_H
#define POKER_ENVIRONMENT_H

#include <vector>
#include <memory>

#include "agent.h"

namespace poker {
    class Environment {
        std::vector<std::unique_ptr<Agent>> agents;
    public:
        Environment(const std::vector<std::string>& agent_types);

        void Run();
    };
}

#endif //POKER_ENVIRONMENT_H
