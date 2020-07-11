#ifndef POKER_ENVIRONMENT_H
#define POKER_ENVIRONMENT_H

#include "agent.h"

namespace poker {
    class Environment {
        std::vector<Agent> agents;
    public:
        Environment(int agent_num);

        void Run();
    };
}

#endif //POKER_ENVIRONMENT_H
