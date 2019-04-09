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

TM::TM(const std::vector<char>& alphabet, const std::vector<char>& tapeAlphabet, const std::vector<TMState*>& states, const TMTransition& transition) :
    alphabet(alphabet), tapeAlphabet(tapeAlphabet), states(states), transition(transition) {}

bool TM::operator()(const std::string& word) const
{
    uint32_t index = 0;
    const TMState* current = start;

    std::vector<char> tape(word.size());
    for(uint32_t i = 0; i < word.size(); i++) tape[i] = word[i];

    while(true)
    {
        const auto next = transition(tape[index], current);

        // new state
        current = std::get<0>(next);
        if     (current->type == accept) return true;
        else if(current->type == reject) return false;

        // new index
        if     (std::get<1>(next) == 'L') index--;
        else if(std::get<1>(next) == 'R') index++;
        else std::cerr << "unknown character\n";
        if(index > tape.size()) tape.reserve(2 * tape.size());

        // new tape character
        tape[index] = std::get<2>(next);
    }
}