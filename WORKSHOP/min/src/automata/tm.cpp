//============================================================================
// @name        : tm.cpp
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "tm.h"
#include <tuple>
#include <iostream>
#include <algorithm>

TM::TM(const std::vector<char>& alphabet, const std::vector<char>& tapeAlphabet, const std::vector<TMState*>& states, const TMTransition& transition) :
    alphabet(alphabet), tapeAlphabet(tapeAlphabet), states(states), transition(transition)
    {
        start = *std::find_if(begin(states), end(states), [](const auto& state){ return state->type == TMState::Type::start; });
    }

bool TM::operator()(const std::string& word) const
{
    uint32_t index = 0;
    const TMState* current = start;

    std::vector<char> tape(word.size());
    for(uint32_t i = 0; i < word.size(); i++) tape[i] = word[i];

    while(true)
    {
        const auto next = transition[{tape[index], current}];

        // new state
        current = std::get<0>(next);
        if(current == nullptr) return false;
        else if(current->type == TMState::Type::accept) return true;
        else if(current->type == TMState::Type::reject) return false;

        // new tape character
        tape[index] = std::get<2>(next);

        // new index
        if(std::get<1>(next) == 'L')
        {
            if(index == 0) throw std::runtime_error("fell off tape\n");
            index--;
        }
        else if(std::get<1>(next) == 'R') index++;
        if(index > tape.size()) tape.reserve(2 * tape.size());
    }
}