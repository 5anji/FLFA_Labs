#include "automata.h"
#include "grammar.h"
#include "lexer.h"
#include "parser.h"

#include <iostream>

int main(int argc, char const** argv) {
    std::string expr = "0*1+12/(13.11112-10.01)";

    if (argc > 1) { expr = std::string(argv[1]); } // for basic expressions (./a.out 2+2)

    Lexer lexer(expr);
    std::vector<Token> tokens = lexer.tokenize();

    if (expr.length() > 0 && tokens.size() == 0) {
        std::cerr << "Error in expression. Recheck it." << std::endl;
    } else {
        for (auto&& token : tokens) {
            std::cout << token << std::endl;
        }
    }

    Parser parser(tokens);
    std::cout << "Result = " << parser.parse() << std::endl;

    return 0;
}
