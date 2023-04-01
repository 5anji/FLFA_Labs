# Lexer and Tokenizer

## Course: Formal Languages & Finite Automata

## Author: Gitlan Gabriel FAF-213

### Theory

The short version of this theory is that in order to create a lexer and tokenizer, we must first create generalized regex expressions to check with our input data. We must then compare the given text with the given regex expression and iterate through the elements one at a time until we have tokenized every portion of the input and must provide specific situations to result in error messages, such as an unequal number of parentheses and multiple operators followed by one another.

### Objectives

1. Understand what lexical analysis [1] is.
2. Get familiar with the inner workings of a lexer/scanner/tokenizer.
3. Implement a sample lexer and show how it works.

### Implementation

A __*special*__ enum, with string conversion compatibility, containing 4 elements, that are described as token categorization:

```c++
DEFINE_ENUM_WITH_STRING_CONVERSIONS(TokenType, (NUMBER)(OPERATOR)(LPARENTHESIS)(RPARENTHESIS))
```

The token class, that has the role of container, for matching the value with its type:

```c++
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
```

After that inside the class Lexer is located a loop that each value is passed through, and gets its category (TokenType):

```c++
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
```

Note: the regex `"(\d+\.?\d*)|([+*/\-()])"` is a helper for determining the float values, and searches the operators inside the given expression.

At the end the class will contain a completed vector of values, that in our main.cpp is used inside a for-range loop, for displaying all clasiffied elements of the given expresiion.

### Conclusions

The output of our executable:

```txt
TOKEN (NUMBER, "0")
TOKEN (OPERATOR, "*")
TOKEN (NUMBER, "1")
TOKEN (OPERATOR, "+")
TOKEN (NUMBER, "12")
TOKEN (OPERATOR, "/")
TOKEN (LPARENTHESIS, "(")
TOKEN (NUMBER, "13.11112")
TOKEN (OPERATOR, "-")
TOKEN (NUMBER, "10.01")
TOKEN (RPARENTHESIS, ")")
```

After all we obtain a pretty ouptput with evident corectess of the output. Pretty interesting lab, where I learned what is lexer and how it works.
