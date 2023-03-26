#pragma once

#include "grammar.h"

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <sstream>

struct Automata {
    std::map<std::pair<std::string, std::string>, std::string> transitions;
    std::vector<std::string> states;
    std::vector<std::string> symbols;
    std::string start_state;
    std::vector<std::string> final_states;

    Automata() {}
    Automata(Grammar& gr) {
        for (Rule v : gr.productions) {
            transitions[std::make_pair(v[0], v[1].substr(0, 1))] = v[1].substr(1, 1);
            states.push_back(v[0]);
            symbols.push_back(v[1].substr(0, 1));
        }

        start_state = "S";
        final_states = gr.terminal_variables;
    }

    bool validate(std::string str) {
        std::string curr = start_state;
        for (auto&& c : str) {
            std::string symbol(1, c);
            if (std::count(symbols.begin(), symbols.end(), symbol) == 0) {
                return true;
            }

            curr = transitions[std::make_pair(curr, symbol)];
        }

        return std::count(final_states.begin(), final_states.end(), curr) > 0;
    }

    static Grammar AutomataToGrammar(const Automata& automata) {
        Grammar grammar;
        grammar.non_terminal_variables = automata.states;
        grammar.terminal_variables = automata.symbols;
        grammar.starting_character = automata.start_state;

        for (auto&& [key, value] : automata.transitions) {
            std::string lhs = key.first;
            std::string rhs = value;
            grammar.productions.push_back({lhs, rhs});
        }

        return grammar;
    }

    bool isDeterministic() const {
        std::set<std::pair<std::string, std::string>> visited;
        for (auto&& [key, value] : transitions) {
            std::string state = key.first;
            std::string symbol = key.second;
            if (visited.count({state, symbol}))
                return false;
            visited.insert({state, symbol});
        }
        return true;
    }

    std::set<std::string> epsilonClosure(const std::set<std::string>& states) {
        std::set<std::string> closure = states;
        std::queue<std::string> frontier;
        for (auto&& state : states) {
            frontier.push(state);
        }
        while (!frontier.empty()) {
            std::string state = frontier.front();
            frontier.pop();
            for (auto&& [key, value] : transitions) {
                if (key.first == state && key.second == "") {
                    if (closure.count(value) == 0) {
                        closure.insert(value);
                        frontier.push(value);
                    }
                }
            }
        }
        return closure;
    }

    std::string stateSetToString(const std::set<std::string>& stateSet) {
        std::stringstream ss;
        ss << "{";
        bool first = true;
        for (auto&& state : stateSet) {
            if (!first) {
                ss << ",";
            }
            ss << state;
            first = false;
        }
        ss << "}";
        return ss.str();
    }

    bool hasIntersection(const std::set<std::string>& set1, const std::set<std::string>& set2) {
        for (auto&& element : set1) {
            if (set2.count(element) > 0) {
                return true;
            }
        }
        return false;
    }

    void convertNFAToDFA() {
        std::map<std::pair<std::string, std::string>, std::string> dfa_transitions;
        std::set<std::string> unprocessed_dfa_states;
        std::map<std::string, std::set<std::string>> nfa_state_subsets;
        std::set<std::string> startNFAStateSubset = epsilonClosure({start_state});
        std::string startDFAState = stateSetToString(startNFAStateSubset);
        nfa_state_subsets[startDFAState] = startNFAStateSubset;
        unprocessed_dfa_states.insert(startDFAState);

        while (!unprocessed_dfa_states.empty()) {
            std::string dfaState = *unprocessed_dfa_states.begin();
            unprocessed_dfa_states.erase(dfaState);

            for (auto&& symbol : symbols) {
                std::set<std::string> nfaStateSubset = move(nfa_state_subsets[dfaState]);
                nfaStateSubset = epsilonClosure(nfaStateSubset);

                std::string nfaStateSubsetString = stateSetToString(nfaStateSubset);
                if (nfa_state_subsets.count(nfaStateSubsetString) == 0) {
                    nfa_state_subsets[nfaStateSubsetString] = nfaStateSubset;
                    unprocessed_dfa_states.insert(nfaStateSubsetString);
                }

                std::string nextDFAState = nfaStateSubsetString;
                dfa_transitions[make_pair(dfaState, symbol)] = nextDFAState;
            }
        }

        transitions = dfa_transitions;

        states.clear();
        final_states.clear();
        for (auto&& [dfaState, nfaStateSubset] : nfa_state_subsets) {
            states.push_back(dfaState);
            if (hasIntersection(nfaStateSubset, std::set(final_states.begin(), final_states.end()))) {
                final_states.push_back(dfaState);
            }
        }
    }
};