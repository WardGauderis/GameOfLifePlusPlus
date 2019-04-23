//============================================================================
// @name        : fsm.cpp
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <algorithm>
#include "fsm.h"

std::map<const Automaton*, char> FSM::states = {};
FSMTransition FSM::transition = {};

FSM::FSM(const Automaton* start) : current(start) {}

void FSM::init(const std::vector<const Automaton*>& states, const FSMTransition& transition)
{
    char index = 65;
    for(const auto& state : states)
    {
        FSM::states[state] = {index};
        index++;
    }
    FSM::transition = transition;
}

void FSM::operator()(const std::string& word) const
{
    current = transition((*current)(word), current);
}

char FSM::getCurrent() const
{
    return states.at(current);
}