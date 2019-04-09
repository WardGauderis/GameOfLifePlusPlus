//============================================================================
// @name        : state.h
// @author      : Thomas Dooms
// @date        : 3/20/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================

#pragma once

#include <string>
#include <vector>
#include <set>
#include <stack>

#include "states.h"
#include "transitions.h"
#include "automaton.h"

class PDA : public Automaton
{
public:
    PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<State*>& states, const PDATransition& transition);
    bool operator()(const std::string& word) const override;

private:
    std::vector<char> alphabet;
    std::vector<char> stackAlphabet;

    std::vector<State*> states;
    PDATransition transition;
    const State* start;

    mutable std::stack<char> stack;
};

