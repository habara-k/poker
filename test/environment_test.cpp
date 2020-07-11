#include "gtest/gtest.h"

#include "environment.h"

using namespace poker;

TEST(env, Env) {
    Environment env(6);

    env.Run();
}
