# Determinism in Finite Automata. Conversion from NFA to DFA. Chomsky Hierarchy

## Course: Formal Languages & Finite Automata

## Author: Gitlan Gabriel FAF-213, Variant 19

```txt
Q = {q0,q1,q2},
∑ = {a,b},
F = {q2},
δ(q0,a) = q1,
δ(q0,a) = q0,
δ(q1,b) = q2,
δ(q0,b) = q0,
δ(q1,b) = q1,
δ(q2,b) = q2.
```

### Theory

 A finite automaton is a device utilized to depict various kinds of processes, similar to a state machine in terms of its structure and purpose. The term "finite" denotes that an automaton possesses a starting state and a set of final states, meaning that any process modeled by an automaton has a clear beginning and end. Depending on the structure of the automaton, there may be cases where a single transition leads to multiple states, resulting in non-determinism. Determinism refers to how predictable a system is when discussing systems theory. When there are random variables present, the system becomes non-deterministic or stochastic. As a result, automata can be categorized as either deterministic or non-deterministic, and it is possible to achieve determinism by utilizing algorithms that alter the automaton's structure. Chomsky Hierarchy is a system of classifying formal languages into four categories based on the complexity of their grammars:

- Regular languages
- Context-free languages
- Context-sensitive languages
- Recursively enumerable languages

A non-deterministic finite automaton NFA is a type of finite automaton that can have multiple transitions for a given input symbol from a particular state. In contrast, a deterministic finite automaton DFA is a type of finite automaton that has only one transition for a given input symbol from a particular state. To convert an NFA to a DFA, we need to create a new DFA that accepts the same language as the NFA. We create a state in the DFA for each possible combination of states in the NFA. The transitions between states in the DFA are determined by examining the possible transitions from each state in the NFA for each input symbol. If a set of states in the NFA includes a final state, then the corresponding state in the DFA is also a final state.

### Objectives

1. Understand what an automaton is and what it can be used for.

2. Continuing the work in the same repository and the same project, the following need to be added:
    a. Provide a function in your grammar type/class that could classify the grammar based on Chomsky hierarchy.

    b. For this you can use the variant from the previous lab.

3. According to your variant number (by universal convention it is register ID), get the finite automaton definition and do the following tasks:

    a. Implement conversion of a finite automaton to a regular grammar.

    b. Determine whether your FA is deterministic or non-deterministic.

    c. Implement some functionality that would convert an NDFA to a DFA.

    d. Represent the finite automaton graphically (Optional, and can be considered as a __*bonus point*__):

    - You can use external libraries, tools or APIs to generate the figures/diagrams.

    - Your program needs to gather and send the data about the automaton and the lib/tool/API return the visual representation.

### Implementation

__still not working__
