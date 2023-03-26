#pragma once

#include <csignal>
#include <cstdlib>
#include <random>
#include <set>
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
};