#include "gtest/gtest.h"

#include "environment.h"

using namespace poker;

TEST(env, Env) {
    Environment env({"check_call", "check_call"});

    env.Run();
}
