#pragma once

#include "lexer.h"

#include <iostream>
#include <stdexcept>
#include <vector>

struct Parser {
public:
    Parser(std::vector<Token> const& tokens)
            : tokens(tokens)
            , token_index(0) {}

    double parse() {
        return expression();
    }

private:
    double expression() {
        double result = term();

        while (token_index < tokens.size() && (tokens[token_index].value == "+" || tokens[token_index].value == "-")) {
            std::string op = tokens[token_index++].value;
            if (op == "+") {
                result += term();
            } else {
                result -= term();
            }
        }

        return result;
    }

    double term() {
        double result = factor();

        while (token_index < tokens.size() && (tokens[token_index].value == "*" || tokens[token_index].value == "/")) {
            std::string op = tokens[token_index++].value;
            if (op == "*") {
                result *= factor();
            } else {
                double divisor = factor();
                if (divisor != 0.0) {
                    result /= divisor;
                } else {
                    std::cerr << "ERR: Division by zero" << std::endl;
                    exit(-1);
                }
            }
        }

        return result;
    }

    double factor() {
        if (token_index >= tokens.size()) {
            std::cerr << "ERR: Unexpected end of input" << std::endl;
            exit(-1);
        }

        Token token = tokens[token_index++];
        if (token.type == TokenType::NUMBER) {
            return std::stod(token.value);
        } else if (token.type == TokenType::LPARENTHESIS) {
            double result = expression();
            if (token_index >= tokens.size() || tokens[token_index++].type != TokenType::RPARENTHESIS) {
                std::cerr << "ERR: No closing parenthesis found" << std::endl;
                exit(-1);
            }
            return result;
        } else {
            std::cerr << "ERR: Invalid token" << std::endl;
            exit(-1);
        }
    }

    std::vector<Token> tokens;
    size_t token_index;
};
