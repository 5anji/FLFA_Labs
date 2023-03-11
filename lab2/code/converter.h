#pragma once
#include "automaton.h"
#include "misc.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

class Converter {
public:
    static Automaton nfaTOdfa(Automaton);
    static std::string autToRE(std::string);
    static void autToRG(std::string);
};

Automaton Converter::nfaTOdfa(Automaton nfa) {
    Automaton dfa;

    dfa.Q.push_back(nfa.Q.front());
    dfa.E = nfa.E;
    std::string* temp_graph = new std::string[nfa.Q.size() * 10 * dfa.E.size()];

    int stop;

    std::list<std::string>::iterator qd, ed, qn, en, state;
    std::list<std::string> states_to_check;
    std::string qs, es, temp, st;
    int i1, i2, j1, j2, start;

    qd = dfa.Q.begin();
    while (qd != dfa.Q.end()) {
        for (ed = dfa.E.begin(); ed != dfa.E.end(); ++ed) {
            j1 = distance(dfa.Q.begin(), qd);
            j2 = distance(dfa.E.begin(), ed);
            *(temp_graph + j1 * dfa.E.size() + j2) = "";

            qs = *qd;
            qs += '<';
            states_to_check.clear();

            for (int i = 0; i < qs.length(); i++) {
                if (i == 0 || qs[i] == '|') {
                    if (i == 0) {
                        start = 0;
                    } else {
                        start = 1;
                    }

                    temp = Misc::readNextToken(qs.substr(i + start, qs.length() - i));
                    if (temp.length() > 0) { states_to_check.push_back(temp); }
                }
            }

            es = *ed;
            en = find(nfa.E.begin(), nfa.E.end(), es);
            i2 = distance(nfa.E.begin(), en);

            for (state = states_to_check.begin(); state != states_to_check.end(); ++state) {
                qs = *state;
                qn = find(nfa.Q.begin(), nfa.Q.end(), qs);
                i1 = distance(nfa.Q.begin(), qn);
                if ((temp_graph + j1 * dfa.E.size() + j2)->length() > 0) { *(temp_graph + j1 * dfa.E.size() + j2) += '|'; }
                if ((nfa.graph + i1 * nfa.E.size() + i2)->length() > 1) {
                    *(temp_graph + j1 * dfa.E.size() + j2) += *(nfa.graph + i1 * nfa.E.size() + i2);
                }
            }

            temp = *(temp_graph + j1 * dfa.E.size() + j2);
            if (temp[temp.length() - 1] == '|') {
                *(temp_graph + j1 * dfa.E.size() + j2) = temp.substr(0, temp.length() - 1);
            }
            *(temp_graph + j1 * dfa.E.size() + j2) = Misc::normalizeComposite(*(temp_graph + j1 * dfa.E.size() + j2));

            temp = *(temp_graph + j1 * dfa.E.size() + j2);

            if ((find(dfa.Q.begin(), dfa.Q.end(), temp) == dfa.Q.end()) && (temp.length() > 0)) {
                dfa.Q.push_back(temp);
            }
        }

        qd++;
    }

    dfa.graph = new std::string[dfa.Q.size() * dfa.E.size()];

    for (int i = 0; i < dfa.Q.size(); i++) {
        for (int j = 0; j < dfa.E.size(); j++) {
            *(dfa.graph + i * dfa.E.size() + j) = *(temp_graph + i * dfa.E.size() + j);
        }
    }

    size_t found;

    for (qd = dfa.Q.begin(); qd != dfa.Q.end(); ++qd) {
        st = *qd;

        for (qn = nfa.F.begin(); qn != nfa.F.end(); ++qn) {
            temp = *qn;
            found = st.find(temp);

            if (found != std::string::npos) {
                if (find(dfa.F.begin(), dfa.F.end(), st) == dfa.F.end()) {
                    dfa.F.push_back(st);
                }
            }
        }
    }

    return dfa;
};

std::string Converter::autToRE(std::string source_name) {
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

    std::list<std::string>::iterator state;

    line = "d(qi,eps) = " + T.front().source + ',';
    T.push_back(Transition(line));

    for (state = F.begin(); state != F.end(); state++) {
        line = "d(" + *state + ",eps) = qf,";
        T.push_back(Transition(line));
    }

    Q.push_back("qf");
    Q.push_back("qi");
    F.clear();
    F.push_back("qf");

    Transition res;
    std::list<Transition> newTransitions;
    std::list<Transition>::iterator tr;
    std::list<std::string> in, out;
    std::list<std::string>::iterator p, q;
    std::string state_to_eliminate;
    std::string alpha, part;
    bool eliminated;

    while (Q.size() > 2) {
        state_to_eliminate = Q.front();
        in.clear();
        out.clear();
        newTransitions.clear();

        for (tr = T.begin(); tr != T.end(); tr++) {
            if (tr->target == state_to_eliminate && tr->source != state_to_eliminate) {
                if (find(in.begin(), in.end(), tr->source) == in.end()) {
                    in.push_back(tr->source);
                }
            }

            if (tr->source == state_to_eliminate && tr->target != state_to_eliminate) {
                if (find(out.begin(), out.end(), tr->target) == out.end()) {
                    out.push_back(tr->target);
                }
            }
        }

        for (p = in.begin(); p != in.end(); p++) {
            for (q = out.begin(); q != out.end(); q++) {
                if (*p == *q) { continue; }
                alpha = "";

                part = "";
                for (tr = T.begin(); tr != T.end(); tr++) {
                    if (tr->source == *p && tr->target == state_to_eliminate) {
                        if (part.length() == 0) {
                            part += tr->character;
                        } else {
                            if (part[0] != '(') { part = '(' + part; }
                            part += '+';
                            part += tr->character;
                        }
                    }
                }
                if (part[0] == '(') { part += ')'; }

                if (part.length() > 0) { alpha += part; }

                part = "";
                for (tr = T.begin(); tr != T.end(); tr++) {
                    if (tr->source == state_to_eliminate && tr->target == state_to_eliminate) {
                        if (part.length() == 0) {
                            part += tr->character;
                            part += '*';
                        } else {
                            if (part[0] != '(') { part = '(' + part; }
                            part += '+';
                            part += tr->character;
                            part += '*';
                        }
                    }
                }
                if (part[0] == '(') { part += ')'; }

                if (part.length() > 0) { alpha += part; }

                part = "";
                for (tr = T.begin(); tr != T.end(); tr++) {
                    if (tr->source == state_to_eliminate && tr->target == *q) {
                        if (part.length() == 0) {
                            part += tr->character;
                        } else {
                            if (part[0] != '(') { part = '(' + part; }
                            part += '+';
                            part += tr->character;
                        }
                    }
                }
                if (part[0] == '(') { part += ')'; }

                if (part.length() > 0) { alpha += part; }

                res.source = *p;
                res.character = alpha;
                res.target = *q;

                newTransitions.push_back(res);
            }
        }

        eliminated = true;

        while (eliminated) {
            eliminated = false;

            for (tr = T.begin(); tr != T.end(); tr++) {

                if ((tr->source == state_to_eliminate || tr->target == state_to_eliminate) || (tr->source == Q.back() && tr->target == F.front())) {
                    res.source = T.front().source;
                    res.character = T.front().character;
                    res.target = T.front().target;

                    T.front().source = tr->source;
                    T.front().character = tr->character;
                    T.front().target = tr->target;

                    tr->source = res.source;
                    tr->character = res.character;
                    tr->target = res.target;

                    T.pop_front();

                    eliminated = true;
                    break;
                }
            }
        }

        while (newTransitions.size() > 0) {
            T.push_back(newTransitions.front());
            newTransitions.pop_front();
        }

        Q.pop_front();
    }

    std::string answer = T.back().character;
    std::string epsilon = "eps";

    for (int i = 0; i < answer.length() - 2; i++) {
        if (answer.substr(i, 3) == epsilon) {
            answer = answer.substr(0, i) + '[' + answer.substr(i, 3) + ']' + answer.substr(i + 3, answer.length() - i - 3);
            i++;
        }
    }

    return answer;
};

void Converter::autToRG(std::string source_name) {
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

    std::list<Transition> ruleset;
    std::list<Transition>::iterator itt, itr;
    Transition newRule;

    bool updated;

    for (itt = T.begin(); itt != T.end(); itt++) {
        updated = false;

        for (itr = ruleset.begin(); itr != ruleset.end(); itr++) {
            if (itt->source == itr->source) {
                itr->target += '|';
                itr->target += itt->character;
                itr->target += itt->target;
                updated = true;
            }
        }

        if (updated) {
            continue;
        } else {
            newRule.source = itt->source;
            newRule.target = itt->character;
            newRule.target += itt->target;

            ruleset.push_back(newRule);
        }
    }

    for (itr = ruleset.begin(); itr != ruleset.end(); itr++) {
        std::cout << itr->source << " -> " << itr->target << '\n';
    }
}
