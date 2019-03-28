//
// Created by thomas on 20/03/2019.
//

#pragma once

#include <string>
#include <vector>
#include <set>
#include <stack>
#include "states.h"
#include "transitions.h"

class PDA
{
public:
    PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<State*>& states, const PDATransition& transition);
    bool operator()(const std::string& word) const;

private:
    bool checkRecursive(const std::stack<char>& stack, const std::string& word, const State* current, uint32_t index) const;

    std::vector<char> alphabet;
    std::vector<char> stackAlphabet;

    std::vector<State*> states;
    PDATransition transition;
    const State* start;
};

