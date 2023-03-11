#pragma once
#include <algorithm>
#include <iostream>
#include <list>

class Misc {
public:
    static std::string readNextToken(std::string);
    static void printList(std::list<std::string>);
    static std::string normalizeComposite(std::string);
};

std::string Misc::readNextToken(std::string part) {
    for (int i = 0; i < part.length(); i++) {
        if (!((part[i] >= 'a' && part[i] <= 'z') || (part[i] >= '0' && part[i] <= '9'))) {
            return part.substr(0, i);
        }
    }

    return "?!";
};

void Misc::printList(std::list<std::string> l) {
    std::list<std::string>::iterator s;

    std::cout << "{ ";
    for (s = l.begin(); s != l.end(); ++s) {
        std::cout << *s << ", ";
    }

    std::cout << "EOL }" << '\n';
};

std::string Misc::normalizeComposite(std::string comp_state) {
    std::list<std::string> states;
    states.clear();
    std::string sorted;
    std::string state;

    int start;
    comp_state += '<';

    for (int i = 0; i < comp_state.length(); i++) {
        if (i == 0 || comp_state[i] == '|') {
            if (i == 0) {
                start = 0;
            } else {
                start = 1;
            }

            state = Misc::readNextToken(comp_state.substr(i + start, comp_state.length() - i));

            if (find(states.begin(), states.end(), state) == states.end()) {
                states.push_back(state);
            }
        }
    }

    states.sort();

    sorted += states.front();
    states.pop_front();
    while (states.size() > 0) {
        sorted += '|';
        sorted += states.front();
        states.pop_front();
    }

    return sorted;
};
