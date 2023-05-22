# Laboratory 5 Parser & Building an abstract Tree

## Course: Formal Languages & Finite Automata

## Author: Gitlan Gabriel FAF-213

### Theory

Parser is the next step after tokenizer, that allows us to use already classified strings in program to give them a logic. In my case the calculation and validation of the given expression.

### Objectives

1. Get familiar with parsing, what it is and how it can be programmed [1].
2. Get familiar with the concept of AST [2].
3. In addition to what has been done in the 3rd lab work do the following:
   1. In case you didn't have a type that denotes the possible types of tokens you need to:
      1. Have a type __*TokenType*__ (like an enum) that can be used in the lexical analysis to categorize the tokens.
      2. Please use regular expressions to identify the type of the token.
   2. Implement the necessary data structures for an AST that could be used for the text you have processed in the 3rd lab work.
   3. Implement a simple parser program that could extract the syntactic information from the input text.

### Implementation

As you can see in the last commit was added a new class(struct) Parser. Let's analyze a bit the private methods of it, that does all the magic.

So the factor() method:

```c++
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
```

The goal af this one is to give a priority to the parentesed expressions. Also to validate closure of the left parenthesis. If not. Throws error.

Next, term() and experssion():

```c++
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
```

As you can see by the condition in the while loop, both of them implements the basic operations between values. Also persists the priority between operators, and, obviously, the catch of exception "division by zero"

### Conclusions/Results

The result of my code defined expression is: `Result = 3.86957`

But if you'll run the executable with `./a.out 2+2` it will give `Result = 4`. I managed to make it work with app arguments. If there is no argument, it will use the default value (`0*1+12/(13.11112-10.01)`)

### References

* Mr. Drumea's Laboratories
* Github
