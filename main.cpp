#include <iostream>
#include <fstream>

#include "environment.h"

using namespace poker;

int main() {
    std::ofstream log("log.txt");
    std::clog.rdbuf(log.rdbuf());

    Environment env({"check_call", "user", "check_call", "check_call", "check_call", "check_call"});
    env.Run();
}