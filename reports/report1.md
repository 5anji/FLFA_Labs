# The title of the work

## Course: Formal Languages & Finite Automata

## Author: Gîtlan Gabriel

----

## Theory

A regular grammar is a type of formal grammar used to generate regular languages, which are a subset of formal languages. It consists of a set of production rules that define how to generate strings of symbols. The rules in a regular grammar are of the form A -> aB or A -> a, where A and B are non-terminal symbols, a is a terminal symbol, and -> denotes a production rule.

Regular grammars are less expressive than other types of grammars, such as context-free grammars, as they can only generate regular languages. However, regular grammars are important in computer science, particularly in the areas of formal language theory and compiler design, as they are used to define and recognize regular expressions, which are widely used in pattern matching and text processing applications.

A finite automaton, also known as a finite state machine, is a mathematical model used to recognize and process regular languages. It consists of a set of states, transitions between those states, a start state, and one or more final states. It operates by consuming input symbols and transitioning between states based on those symbols until it reaches an accepting state or a non-accepting state. It is used in various areas of computer science, such as compiler design, natural language processing, and digital circuit design.

## Objectives

1) Understand what a language is and what it needs to have in order to be considered a formal one.

2) Provide the initial setup for the evolving project that you will work on during this semester. I said project because usually at lab works, I encourage/impose students to treat all the labs like stages of development of a whole project. Basically you need to do the following:
    - Create a local && remote repository of a VCS hosting service (let us all use Github to avoid unnecessary headaches);
    - Choose a programming language, and my suggestion would be to choose one that supports all the main paradigms;
    - Create a separate folder where you will be keeping the report. This semester I wish I won't see reports alongside source code files, fingers crossed;

3) According to your variant number (by universal convention it is register ID), get the grammar definition and do the following tasks:
    - Implement a type/class for your grammar;
    - Add one function that would generate 5 valid strings from the language expressed by your given grammar;
    - Implement some functionality that would convert and object of type Grammar to one of type Finite Automaton;
    - For the Finite Automaton, please add a method that checks if an input string can be obtained via the state transition from it;

## Implementation description

So, I decided to implement this in modern C++. I respected the initial structure of separating methods inside classes. Also I beautified it by adding read-only encapsulation. In another words, after creating object through this constructor:

```c++
Grammar() {
    VN = {"S", "A", "B", "C"};
    VT = {"a", "b"};

    P["S"] = {"aA"};
    P["A"] = {"bS", "aB"};
    P["B"] = {"bC"};
    P["C"] = {"aA", "b"};
}
```

...you will not be able to modify fields, inside class.

After that I implemented the string generator:

```c++
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
```

Here I just access every element of `std::map`, and filter the repeated elements. And in the same function I'm displaying it out.

And at the last the function that checks if the string is valid:

```c++
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
```

Actually, looks creepy, but it's more optimized to make it all by `std::pair`. Just a filter for each character, in the string provided.

See the full source code [here](../code/main.cpp)

## Conclusions

So in this lab, I learned what is a formal language, how to implement the Finite Automata and some functions that it must have. Also how to make the class more flexible, to grammar modification. In another words it is available for any grammar, provided.

## References

- [geeksforgeeks.org](https://www.geeksforgeeks.org/introduction-of-finite-automata/)

- [wikipedia.org](https://en.wikipedia.org/wiki/Finite-state_machine)
