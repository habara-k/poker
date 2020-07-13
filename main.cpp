#include <iostream>

#include "environment.h"

using namespace poker;

int main() {
    Environment env({"user", "check_call"});

    env.Run();
}