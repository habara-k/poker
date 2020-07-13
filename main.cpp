#include <iostream>
#include <fstream>

#include "environment.h"

using namespace poker;

int main() {
    std::ofstream log("log.txt");
    std::streambuf* old = std::clog.rdbuf(log.rdbuf());
    std::clog.rdbuf(old);

    Environment env({"check_call", "user", "check_call", "check_call", "check_call", "check_call"});
    env.Run();

    return 0;
}