#include <iostream>
#include <fstream>

#include "environment.h"

using namespace poker;

int main() {
    std::ofstream log("log.txt");
    std::streambuf* old = std::clog.rdbuf(log.rdbuf());

    Environment env({"check_fold", "user", "check_fold", "check_call", "check_fold", "check_call"});
    env.Run();

    std::clog.rdbuf(old);

    return 0;
}