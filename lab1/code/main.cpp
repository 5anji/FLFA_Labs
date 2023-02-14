#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

// variant 18

class Grammar {
    std::vector<std::string> VN;
    std::vector<std::string> VT;
    std::map<std::string, std::vector<std::string>> P;

public:
    Grammar() {
        VN = {"S", "A", "B", "C"};
        VT = {"a", "b"};

        P["S"] = {"aA", "aB"};
        P["A"] = {"bS"};
        P["B"] = {"aC"};
        P["C"] = {"a", "bS"};
    }

    std::vector<std::string>& get_VN() {
        return VN;
    }
    std::vector<std::string>& get_VT() {
        return VT;
    }

    std::map<std::string, std::vector<std::string>>& get_P() {
        return P;
    }
    void generate_strings() {
        int count = 0;
        while (count < 5) {
            std::string curr = "S";
            std::string res = "";

            while (std::find(VN.begin(), VN.end(), curr) != VN.end()) {
                int random = rand() % P[curr].size();
                res += P[curr][random];
                curr = res.back();
            }

            if (std::find(VT.begin(), VT.end(), curr) != VT.end()) {
                std::cout << res << std::endl;
                count++;
            }
        }
    }
};

class FiniteAutomaton {
    std::map<std::pair<std::string, std::string>, std::string> transitions;
    std::vector<std::string> states;
    std::vector<std::string> symbols;
    std::string start_state;
    std::vector<std::string> final_states;

public:
    FiniteAutomaton() = default;
    FiniteAutomaton(Grammar& gr) {
        for (auto [key, value] : gr.get_P()) {
            for (std::string v : value) {
                transitions[make_pair(key, v.substr(0, 1))] = v.substr(1, 1);
                states.push_back(key);
                symbols.push_back(v.substr(0, 1));
            }
        }

        start_state = "S";
        final_states = gr.get_VN();
    }

    bool is_valid_string(std::string str) {
        std::string curr = start_state;
        for (auto&& c : str) {
            std::string symbol(1, c);
            if (std::count(symbols.begin(), symbols.end(), symbol) == 0) {
                return false;
            }

            curr = transitions[std::make_pair(curr, symbol)];
        }

        return std::count(final_states.begin(), final_states.end(), curr) > 0;
    }
};

int main() {
    Grammar g;
    // g.generate_strings();

    FiniteAutomaton fa(g);
    std::cout << fa.is_valid_string("a") << std::endl;
    std::cout << fa.is_valid_string("B") << std::endl;
    std::cout << fa.is_valid_string("ab") << std::endl;
    std::cout << fa.is_valid_string("aba") << std::endl;
    
    return 0;
}
