#include "converter.h"

#include <iostream>

int main() {
    std::cout << Automaton::NFAorDFA("automaton.txt") << std::endl;

    return 0;
};