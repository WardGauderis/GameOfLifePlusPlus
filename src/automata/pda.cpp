//============================================================================
// @name        : state.h
// @author      : Thomas Dooms
// @date        : 3/20/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================
#include <algorithm>
#include "pda.h"

PDA::PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<const State*>& states, const PDATransition& transition)
: alphabet(alphabet), stackAlphabet(stackAlphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
    stack.push(stackAlphabet.back());
}

bool PDA::operator()(const std::string& word) const
{
    const State* current = start;

    for(char c : word)
    {
        const auto next = transition(c, current);
        current = std::get<0>(next);
        char push = std::get<1>(next);
        char pop = std::get<2>(next);

        if(push != alphabet.back()) stack.push(push);

        if(pop  != alphabet.back() and stack.top() == pop) stack.pop();
        else return false;
    }
    return current->accepting;
}

