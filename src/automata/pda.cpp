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

PDA::PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<const PDAState*>& states, const PDATransition& transition)
: alphabet(alphabet), stackAlphabet(stackAlphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}

bool PDA::operator()(const std::string& word) const
{
    stack = {};
    const PDAState* current = start;
    for(char c : word)
    {
        const auto next = transition[{c, current}];
        current = std::get<0>(next);
        char push = std::get<1>(next);
        char pop = std::get<2>(next);

        // we first pop, then push, goes against convention but we don't care.
        if(!stack.empty() and pop != stackAlphabet.back())
        {
            if(stack.top() == pop) stack.pop();
            else return false;
        }

        if(push != stackAlphabet.back()) stack.push(push);
    }
    return stack.empty();
}

