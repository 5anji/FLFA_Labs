#pragma once

#include <boost/preprocessor.hpp>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
    case elem : return BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
    enum name {                                                               \
        BOOST_PP_SEQ_ENUM(enumerators)                                        \
    };                                                                        \
                                                                              \
    inline const char* ToString(name v)                                       \
    {                                                                         \
        switch (v)                                                            \
        {                                                                     \
            BOOST_PP_SEQ_FOR_EACH(                                            \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
                name,                                                         \
                enumerators                                                   \
            )                                                                 \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                                     \
    }

DEFINE_ENUM_WITH_STRING_CONVERSIONS(TokenType, (NUMBER)(OPERATOR)(LPARENTHESIS)(RPARENTHESIS))

class Token {
public:
    TokenType type;
    std::string value;

    Token() = default;

    Token(TokenType type, std::string const& value)
            : type(type)
            , value(value) {}

    friend std::ostream& operator<<(std::ostream& os, Token const& token) {
        os << "TOKEN (" << ToString(token.type) << ", \"" << token.value << "\")";
        return os;
    }
};

class Lexer {
private:
    std::vector<Token> tokens;
    std::string text;
    int position{0};

public:
    Lexer(std::string const& text)
            : text(text) {}

    std::vector<Token> tokenize() {
        std::regex pattern("(\\d+\\.?\\d*)|([+*/\\-()])");
        std::smatch match;
        int lpar = 0, rpar = 0;
        int continuity = -1;
        while (std::regex_search(text, match, pattern)) {
            for (size_t i = 1; i < match.size(); i++) {
                if (!match[i].str().empty()) {
                    TokenType type;
                    std::string value = match[i].str();

                    if (i == 1) {
                        type = TokenType::NUMBER;
                        if (continuity == 0) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 0;
                    } else if (value == "(") {
                        type = TokenType::LPARENTHESIS;
                        lpar++;
                    } else if (value == ")") {
                        type = TokenType::RPARENTHESIS;
                        rpar++;
                    } else if (value == "+" || value == "-" || value == "*" || value == "/") {
                        type = TokenType::OPERATOR;
                        if (continuity == 1) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 1;
                    }
                    Token token(type, value);
                    tokens.push_back(token);
                    position += match[i].length();
                    break;
                }
            }
            text = match.suffix().str();
        }
        if (lpar != rpar) {
            tokens.clear();
        }
        return tokens;
    }
};