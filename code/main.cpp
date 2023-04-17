#include "automata.h"
#include "grammar.h"
#include "lexer.h"

// #include <fstream>
#include <iostream>

int main() {
    // int counter = 0;
    // std::fstream file("./variants.txt");
    // std::string variant = "19";
    // std::string line;

    // while (getline(file, line)) {
    //     if (line != "") variant += line + "\n";
    //     if (line.find('}') != std::string::npos) counter++;
    //     if (counter % 3 == 0 && counter > 0) {
    //         if (variant != "\r\n" && variant.find("{") != std::string::npos) {
    //             Grammar grammar(variant);
    //             Automata automata(grammar);
    //             Automata::AutomataToGrammar(automata);
    //             std::cout << grammar.clasify() << std::endl;
    //             variant = "";
    //         }
    //     }
    // }
    // file.close();

    // Automata automata2;
    // automata2.final_states = {"q2"};
    // automata2.start_state = "q0";
    // automata2.symbols = {"q0", "q1", "q2"};
    // automata2.states = {"a", "b"};
    // automata2.transitions = {
    //         {{"q0", "a"}, "q1"},
    //         {{"q0", "a"}, "q0"},
    //         {{"q1", "b"}, "q2"},
    //         {{"q0", "b"}, "q0"},
    //         {{"q1", "b"}, "q1"},
    //         {{"q2", "b"}, "q2"}};

    // std::cout << "This automata is: " << (automata2.isDeterministic() ? "Deterministic" : "Non-Deterministic") << std::endl;
    // automata2.convertNFAToDFA();

    // Lexer lexer("0*1+12/(13.11112-10.01)");
    // std::vector<Token> tokens = lexer.tokenize();
    // for (auto&& token : tokens) {
    //     std::cout << token << std::endl;
    // }

    /// \@lab: 4, variant 19
    Grammar gr;
    gr.terminal_variables.push_back("S");
    gr.terminal_variables.push_back("A");
    gr.terminal_variables.push_back("B");
    gr.terminal_variables.push_back("C");
    gr.terminal_variables.push_back("E");

    gr.non_terminal_variables.push_back("a");
    gr.non_terminal_variables.push_back("b");

    gr.starting_character = "S";

    gr.productions.push_back(Rule("S", "dB")); // 1
    gr.productions.push_back(Rule("S", "B")); // 2
    gr.productions.push_back(Rule("A", "d")); // 3
    gr.productions.push_back(Rule("A", "dS")); // 4
    gr.productions.push_back(Rule("A", "aAdCB")); // 5
    gr.productions.push_back(Rule("B", "aC")); // 6
    gr.productions.push_back(Rule("B", "bA")); // 7
    gr.productions.push_back(Rule("B", "AC")); // 8
    gr.productions.push_back(Rule("C", "ε")); // 9
    gr.productions.push_back(Rule("E", "AS")); // 10

    gr.toCNF();
}
