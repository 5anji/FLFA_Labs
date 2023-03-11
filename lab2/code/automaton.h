#pragma once
#include "misc.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

class Transition {
public:
    std::string source, character, target;

    Transition(){};

    Transition(std::string fun) {
        for (int i = 0; i < fun.length(); i++) {
            if (fun[i] == '(') {
                source = Misc::readNextToken(fun.substr(i + 1, fun.length() - i));
            } else if ((fun[i] == ',') && (i != fun.length() - 1)) {
                character = Misc::readNextToken(fun.substr(i + 1, fun.length() - i));
            } else if (fun[i] == '=') {
                target = Misc::readNextToken(fun.substr(i + 2, fun.length() - i - 1));
            }
        }
    };
};

class Automaton {
public:
    std::string* graph;
    std::list<std::string> Q, E, F;
    std::list<Transition> G;

    static std::string NFAorDFA(std::string);

    Automaton(){};

    Automaton(std::string source_name) {
        std::ifstream language(source_name);
        std::string line;

        getline(language, line);
        for (int i = 0; i < line.length() - 1; i++) {
            if ((line[i] == '{') || (line[i] == ',')) {
                Q.push_back(Misc::readNextToken(line.substr(i + 1, line.length() - i)));
            }
        }

        getline(language, line);
        for (int i = 0; i < line.length() - 1; i++) {
            if ((line[i] == '{') || (line[i] == ',')) {
                E.push_back(Misc::readNextToken(line.substr(i + 1, line.length() - i)));
            }
        }

        getline(language, line);
        for (int i = 0; i < line.length() - 1; i++) {
            if ((line[i] == '{') || (line[i] == ',')) {
                F.push_back(Misc::readNextToken(line.substr(i + 1, line.length() - i)));
            }
        }

        graph = new std::string[Q.size() * E.size()];

        while (!language.eof()) {
            getline(language, line);
            G.push_back(Transition(line));
        }

        std::list<Transition>::iterator t;
        std::list<std::string>::iterator q, e;
        int i1, i2;
        std::string target_state;

        for (q = Q.begin(); q != Q.end(); ++q) {
            for (e = E.begin(); e != E.end(); ++e) {
                target_state.clear();

                for (t = G.begin(); t != G.end(); ++t) {
                    if (t->source == *q && t->character == *e) {
                        if (target_state.length() > 0) {
                            target_state += '|';
                        }

                        target_state += t->target;
                    }
                }

                i1 = distance(Q.begin(), q);
                i2 = distance(E.begin(), e);

                *(graph + i1 * E.size() + i2) = target_state;
            }
        }

        for (int i = 0; i < Q.size(); i++) {
            for (int j = 0; j < E.size(); j++) {
                if ((graph + i * E.size() + j)->length() == 0) {
                    *(graph + i * E.size() + j) = '-';
                } else {
                    *(graph + i * E.size() + j) = Misc::normalizeComposite(*(graph + i * E.size() + j));
                }
            }
        }
    };

    void displayConfig();
};

void Automaton::displayConfig() {
    std::list<std::string>::iterator state, character;
    int i1, i2;

    std::cout << "Q = ";
    Misc::printList(Q);
    std::cout << "E = ";
    Misc::printList(E);
    std::cout << "F = ";
    Misc::printList(F);

    for (state = Q.begin(); state != Q.end(); ++state) {
        for (character = E.begin(); character != E.end(); ++character) {
            i1 = distance(Q.begin(), state);
            i2 = distance(E.begin(), character);

            if ((graph + i1 * E.size() + i2)->length() > 1) {
                std::cout << "d(" << *state << ',' << *character << ") = " << *(graph + i1 * E.size() + i2) << '\n';
            }
        }
    }
};

std::string Automaton::NFAorDFA(std::string source_name) {
    std::list<std::string> Q, E, F;
    std::list<Transition> T;

    std::ifstream language(source_name);
    std::string line;

    getline(language, line);
    for (int i = 0; i < line.length() - 1; i++) {
        if ((line[i] == '{') || (line[i] == ',')) {
            Q.push_back(Misc::readNextToken(line.substr(i + 1, line.length() - i)));
        }
    }

    getline(language, line);
    for (int i = 0; i < line.length() - 1; i++) {
        if ((line[i] == '{') || (line[i] == ',')) {
            E.push_back(Misc::readNextToken(line.substr(i + 1, line.length() - i)));
        }
    }

    getline(language, line);
    for (int i = 0; i < line.length() - 1; i++) {
        if ((line[i] == '{') || (line[i] == ',')) {
            F.push_back(Misc::readNextToken(line.substr(i + 1, line.length() - i)));
        }
    }

    while (!language.eof()) {
        getline(language, line);
        T.push_back(Transition(line));
    }

    int count[Q.size()][E.size()];

    for (int i = 0; i < Q.size(); i++) {
        for (int j = 0; j < E.size(); j++) {
            count[i][j] = 0;
        }
    }

    std::list<Transition>::iterator tr;
    std::list<std::string>::iterator qi, ei;
    std::string qs, es;
    int i1, i2;

    for (tr = T.begin(); tr != T.end(); tr++) {
        qs = tr->source;
        es = tr->character;
        qi = find(Q.begin(), Q.end(), qs);
        ei = find(E.begin(), E.end(), es);
        i1 = distance(Q.begin(), qi);
        i2 = distance(E.begin(), ei);

        count[i1][i2]++;

        if (count[i1][i2] > 1) {
            line = "NFA";
            return line;
        }
    }

    line = "DFA";
    return line;
};
