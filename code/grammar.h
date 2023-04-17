#pragma once

#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

struct Rule {
    std::string Left;
    std::string Right;

    std::string operator[](const int& index) {
        if (index == 0)
            return Left;
        else
            return Right;
    }

    Rule(const std::string& left, const std::string& right) {
        Left = left;
        Right = right;
    }
};

std::ostream& operator<<(std::ostream& os, Rule& rule) {
    os << (rule[0]) << std::string(" -> ") << (rule[1]);
    return os;
}

struct Grammar {
    std::vector<std::string> non_terminal_variables;
    std::vector<std::string> terminal_variables;
    std::vector<Rule> productions;
    std::string starting_character;

    std::string generate_word() {
        int count = 1;
        std::vector<Rule> toBeChanged;
        std::string word = starting_character;

        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};

        while (count != 0) {
            count = 0;
            toBeChanged = {};
            int index = 0;
            for (Rule rule : productions) {
                if (word.find(rule.Left) != std::string::npos) {
                    count++;
                    toBeChanged.emplace_back(rule);
                }
            }

            if (count != 0) {
                index = mersenne_engine() % count;
                replace(word, toBeChanged[index][0], toBeChanged[index][1]);
            }
        }
        return word;
    }

    bool isType3Grammar(const std::vector<Rule>& P) {
        for (const Rule& prod : P) {
            if (prod.Right.length() > 1 || !isupper(prod.Left[0]) || !islower(prod.Right[0])) {
                return false;
            }
        }
        return true;
    }

    bool isType2Grammar(const std::vector<Rule>& P) {
        for (const Rule& prod : P) {
            if (prod.Right.empty() || !isupper(prod.Left[0])) {
                return false;
            }
        }
        return true;
    }

    bool isType1Grammar(const std::vector<Rule>& P) {
        for (const Rule& prod : P) {

            if (prod.Right.length() < 2 || !isupper(prod.Left[0]) || prod.Right[0] == prod.Left[0]) {
                return false;
            }
            bool foundNonterminal = false;
            for (char c : prod.Right) {
                if (isupper(c)) {
                    if (foundNonterminal) {
                        return false;
                    }
                    foundNonterminal = true;
                } else {
                    if (!foundNonterminal) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    std::string clasify() {
        if (isType3Grammar(productions)) {
            return "Type 3";
        } else if (isType2Grammar(productions)) {
            return "Type 2";
        } else if (isType1Grammar(productions)) {
            return "Type 1";
        } else {
            return "Type 0";
        }
    }

    Grammar() {}
    Grammar(const std::string& variant) {
        std::string variant_copy = variant;

        auto x1 = variant_copy.find("{");
        auto x2 = variant_copy.find("}");
        auto VT = variant_copy.substr(x1 + 1, x2 - x1 - 1);
        while (VT.find(",") != std::string::npos) replace(VT, ", ", "");
        for (int i = 0; i < VT.size(); i++) non_terminal_variables.emplace_back(std::to_string(VT[i]));
        variant_copy.replace(0, variant_copy.find("},") + 4, "");

        x1 = variant_copy.find("{");
        x2 = variant_copy.find("}");
        auto VN = variant_copy.substr(x1 + 1, x2 - x1 - 1);
        while (VN.find(",") != std::string::npos) replace(VN, ", ", "");
        for (int i = 0; i < VN.size(); i++) terminal_variables.emplace_back(std::to_string(VN[i]));

        variant_copy.replace(0, variant_copy.find("},") + 4, "");
        variant_copy.replace(0, variant_copy.find("{") + 4, "");
        variant_copy.replace(variant_copy.size() - 3, 1, "");
        while (variant_copy.find(" ") != std::string::npos) replace(variant_copy, " ", "");

        std::string tmp;
        while (variant_copy.find("\n") != std::string::npos) {
            productions.emplace_back(Rule(variant_copy.substr(0, 1), variant_copy.substr(4, variant_copy.find("\n") - 4)));
            replace(productions[productions.size() - 1].Right, "\r", "");
            variant_copy.replace(0, variant_copy.find("\n") + 1, "");
            if (variant_copy.size() < 3) break;
        }

        starting_character = "S";
    }
    void toCNF() {
        eliminateEpsilonProductions();
        eliminateRenaming();
        eliminateInaccessibleSymbols();
        eliminateNonproductiveSymbols();
        convertToChomskyNormalForm();

        std::sort(productions.begin(), productions.end(), [](const Rule& a, const Rule& b) {
            if (a.Left == b.Left) {
                return a.Right < b.Right;
            }
            return a.Left < b.Left;
        });
        auto it = std::unique(productions.begin(), productions.end(), [](const Rule& a, const Rule& b) {
            return a.Left == b.Left && a.Right == b.Right;
        });
        productions.erase(it, productions.end());
        for (const auto& r : productions) {
            std::cout << r.Left << " -> " << r.Right << std::endl;
        }
    }

private:
    void eliminateEpsilonProductions() {
        std::unordered_set<std::string> epsilon_vars;

        for (const auto& r : productions) {
            if (r.Right == "ε") {
                epsilon_vars.insert(r.Left);
            }
        }
        while (!epsilon_vars.empty()) {
            std::string var = *epsilon_vars.begin();
            epsilon_vars.erase(epsilon_vars.begin());
            for (auto it = productions.begin(); it != productions.end();) {
                if (it->Right == "ε") {
                    it = productions.erase(it);
                } else if (it->Right.find(var) != std::string::npos) {
                    std::vector<std::string> combinations = {""};
                    for (const auto& c : it->Right) {
                        if (std::string(1, c) == var) {
                            size_t n = combinations.size();
                            for (size_t i = 0; i < n; i++) {
                                combinations.push_back(combinations[i]);
                                combinations.back().push_back(c);
                                combinations[i].push_back(' ');
                            }
                        } else {
                            for (auto& s : combinations) {
                                s.push_back(c);
                            }
                        }
                    }
                    for (const auto& s : combinations) {
                        if (s != it->Right) {
                            productions.push_back({it->Left, s});
                        }
                    }

                    it = productions.erase(it);
                } else {
                    ++it;
                }
            }
            for (auto it = productions.begin(); it != productions.end();) {
                if (it->Left == var && it->Right == "ε") {
                    it = productions.erase(it);
                } else if (it->Right.find(var) != std::string::npos) {
                    productions.push_back({it->Left, it->Right});
                    it = productions.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    void eliminateRenaming() {
        std::map<std::string, std::vector<std::vector<std::string>>> replacements;
        for (const auto& r : productions) {
            if (r.Right.size() == 1 && isupper(r.Right[0])) {
                replacements.emplace(r.Left, std::vector<std::vector<std::string>>{{r.Right}});
            } else {
                std::vector<std::vector<std::string>> right_parts;
                std::vector<std::string> current_part;
                for (char c : r.Right) {
                    if (isupper(c)) {
                        if (!current_part.empty()) {
                            right_parts.push_back(current_part);
                            current_part.clear();
                        }
                        current_part.push_back(std::string(1, c));
                    } else {
                        current_part.push_back(std::string(1, c));
                    }
                }
                if (!current_part.empty()) {
                    right_parts.push_back(current_part);
                }
                if (replacements.count(r.Left) == 0) {
                    replacements.emplace(r.Left, std::vector<std::vector<std::string>>{right_parts[0]});
                } else {
                    replacements[r.Left].push_back(right_parts[0]);
                }
                for (size_t i = 1; i < right_parts.size(); i++) {
                    replacements[r.Left].push_back(right_parts[i]);
                }
            }
        }

        std::vector<Rule> new_productions;
        for (const auto& r : productions) {
            if (replacements.count(r.Left) == 0) {
                new_productions.push_back(r);
            } else {
                std::vector<std::vector<std::string>> replacement_parts = replacements[r.Left];
                for (const auto& p : replacement_parts) {
                    std::string new_right;
                    for (const auto& s : p) {
                        new_right += s;
                    }
                    new_productions.push_back({r.Left, new_right});
                }
            }
        }
        productions = new_productions;
    }

    void eliminateInaccessibleSymbols() {
        std::vector<std::string> reachable_symbols{starting_character};
        bool found_new_symbol = true;
        while (found_new_symbol) {
            found_new_symbol = false;
            for (const auto& r : productions) {
                if (std::find(reachable_symbols.begin(), reachable_symbols.end(), r.Left) != reachable_symbols.end()) {
                    for (char c : r.Right) {
                        if (isupper(c)) {
                            std::string var(1, c);
                            if (std::find(reachable_symbols.begin(), reachable_symbols.end(), var) == reachable_symbols.end()) {
                                reachable_symbols.push_back(var);
                                found_new_symbol = true;
                            }
                        }
                    }
                }
            }
        }

        std::vector<Rule> new_productions;
        for (const auto& r : productions) {
            bool is_reachable = true;
            for (char c : r.Right) {
                if (isupper(c)) {
                    std::string var(1, c);
                    if (std::find(reachable_symbols.begin(), reachable_symbols.end(), var) == reachable_symbols.end()) {
                        is_reachable = false;
                        break;
                    }
                }
            }
            if (is_reachable) {
                new_productions.push_back(r);
            }
        }
        productions = new_productions;
    }

    void eliminateNonproductiveSymbols() {
        std::vector<std::string> productive_symbols{non_terminal_variables};
        bool found_new_symbol = true;
        while (found_new_symbol) {
            found_new_symbol = false;
            for (const auto& r : productions) {
                if (std::find(productive_symbols.begin(), productive_symbols.end(), r.Left) != productive_symbols.end()) {
                    bool is_productive = true;
                    for (char c : r.Right) {
                        if (isupper(c)) {
                            std::string var(1, c);
                            if (std::find(productive_symbols.begin(), productive_symbols.end(), var) == productive_symbols.end()) {
                                is_productive = false;
                                break;
                            }
                        }
                    }
                    if (is_productive) {
                        std::string var = std::string() + r.Left;
                        if (std::find(productive_symbols.begin(), productive_symbols.end(), var) == productive_symbols.end()) {
                            productive_symbols.push_back(var);
                            found_new_symbol = true;
                        }
                    }
                }
            }
        }
        std::vector<std::string> new_non_terminal_variables;
        std::vector<Rule> new_productions;
        for (const auto& var : productive_symbols) {
            if (std::find(non_terminal_variables.begin(), non_terminal_variables.end(), var) != non_terminal_variables.end()) {
                new_non_terminal_variables.push_back(var);
            }
            for (const auto& r : productions) {
                if (r.Left == var || find(r.Right.begin(), r.Right.end(), var[0]) != r.Right.end()) {
                    new_productions.push_back(r);
                }
            }
        }
        non_terminal_variables = new_non_terminal_variables;
        productions = new_productions;
    }

    void convertToChomskyNormalForm() {
        std::map<std::string, int> indices;
        int index = 0;
        for (const std::string& var : non_terminal_variables) {
            indices[var] = index++;
        }
        for (Rule& r : productions) {
            if (r.Right.size() > 2) {
                std::string newVar = "X" + std::to_string(index++);
                int length = r.Right.length();
                for (int i = 0; i < length - 2; i++) {
                    std::string newRuleVar = (i == 0) ? r.Left : newVar;
                    std::string newRuleRight = r.Right.substr(i, 2);
                    productions.push_back({newRuleVar, newRuleRight});
                    r.Right.replace(i, 2, newRuleVar);
                }
                productions.push_back({newVar, r.Right.substr(length - 2, 2)});
                r.Right.replace(length - 2, 2, newVar);
            }
        }
    }
};