//============================================================================
// @name        : NFA.h
// @author      : Thomas Dooms
// @date        : 3/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include <vector>
#include <algorithm>

#include "states.h"
#include "transitions.h"

struct NFA
{
    NFA(const std::vector<char>& alphabet, const std::vector<const State*>& states, const NFATransition& transition, const std::string& type)
    : alphabet(alphabet), states(states), transition(transition), type(type)
    {
        start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
    }
    ~NFA()
    {
        for(const State* state : states) delete state;
    }

    void ecloseCurrent(std::set<const State*>& current) const
    {
        if(type != "enfa") return;
        char epsilon = alphabet.back();
        for(const State* state : current)
        {
            const auto temp = transition[{epsilon, state}];
            current.insert(begin(temp), end(temp));
        }
    }

    std::vector<char> alphabet;
    std::vector<const State*> states;
    const State* start;
    NFATransition transition;
    std::string type;
};





