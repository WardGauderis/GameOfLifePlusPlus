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

// https://en.wikipedia.org/wiki/Deterministic_pushdown_automaton
class PDA : public Automaton
{
public:
    PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<const PDAState*>& states, const PDATransition& transition);
    bool operator()(const std::string& word) const override;

private:
    std::vector<char> alphabet;
    std::vector<char> stackAlphabet;

    std::vector<const PDAState*> states;
    PDATransition transition;
    const PDAState* start;

    mutable std::stack<char> stack;
};

