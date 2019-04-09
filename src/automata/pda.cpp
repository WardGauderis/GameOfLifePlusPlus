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

PDA::PDA(const std::vector<char>& alphabet, const std::vector<char>& stackAlphabet, const std::vector<State*>& states, const PDATransition& transition)
: alphabet(alphabet), stackAlphabet(stackAlphabet), states(states), transition(transition)
{
    start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->starting; });
}

bool PDA::operator()(const std::string& word) const
{
    std::stack<char> stack;
    stack.push(stackAlphabet.back());
    return checkRecursive(stack, word, start, 0);
}

bool PDA::checkRecursive(const std::stack<char>& stack, const std::string& word, const State* current, const uint32_t index) const
{
    if(index >= word.size()) return current->accepting and stack.top() == stackAlphabet.back();
    const char characters[2] = { alphabet.back(), word[index] };

    for(uint32_t i = 0; i < 2; i++)
    {
        const auto next = transition(characters[i], current);
        for(const auto& tuple : next)
        {
            char push = std::get<1>(tuple);
            char pop  = std::get<2>(tuple);

            std::stack<char> newStack = stack;
            if(push != alphabet.back())
            {
                newStack.push(push);
            }
            if(pop != alphabet.back())
            {
                // if pop != epsilon, we need to ensure the top of the stack is identical
                if(newStack.top() != pop) continue;
                newStack.pop();
            }
            if(checkRecursive(newStack, word, std::get<0>(tuple), index + i)) return true;
        }
    }

    // if all fails return false, this means there may be no transitions or something
    return false;
}
