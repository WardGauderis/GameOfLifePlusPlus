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

FSM::FSM(const Automaton* start) : current(start) {}

FSM::~FSM()
{
    for(const auto& state: states) delete state;
}

void FSM::operator()(const std::string& word) const
{
    current = transition((*current)(word), current);
}

char FSM::getState() const
{
    return 65 + (std::find(begin(states), end(states), current) - begin(states));
}