# Chomsky Normal Form

## Course: Formal Languages & Finite Automata

## Author: Gitlan Gabriel FAF-213

### Theory

In formal language theory, a context-free grammar, G, is said to be in Chomsky normal form if all of its production rules are of the form: where A, B, and C are nonterminal symbols, the letter a is a terminal symbol, S is the start symbol, and ε denotes the empty string.

### Objectives

1. Learn about Chomsky Normal Form (CNF) [1].
2. Get familiar with the approaches of normalizing a grammar.
3. Implement a method for normalizing an input grammar by the rules of CNF.
    1. The implementation needs to be encapsulated in a method with an appropriate signature (also ideally in an appropriate class/type).
    2. The implemented functionality needs executed and tested.
    3. A BONUS point will be given for the student who will have unit tests that validate the functionality of the project.
    4. Also, another BONUS point would be given if the student will make the aforementioned function to accept any grammar, not only the one from the student's variant.

### Implementation

On the implementation of the solution to this laboratory work was harder than expected but in the end I think it ended up working well. But I at the end reached at a solution:

A helper function for calling each step of CNF:

```c++
void toCNF() {
    eliminateEpsilonProductions();
    eliminateRenaming();
    eliminateInaccessibleSymbols();
    eliminateNonproductiveSymbols();
    convertToChomskyNormalForm();

    ...

    for (const auto& r : productions) {
        std::cout << r.Left << " -> " << r.Right << std::endl;
    }
}
```

Function that eliminates eps. prods:

```c++
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
```

Function to eliminate remainning part and replace with thei equivalent parts:

```c++
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
```

Functions to remove unnecessary symbols:

```c++
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
```

Now after applying all above filters, we can convert it into Chomsky normal form:

```c++
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
```

### Conclusions

The output of our executable:
(variant 19)
```txt
A -> a
B -> a
B -> b
```

After all we obtain a pretty ouptput with evident corectess of the output. Pretty interesting lab, where I learned what is lexer and how it works.
