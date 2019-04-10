//============================================================================
// @name        : fsm.cpp
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "fsm.h"

FSM::FSM(const std::vector<std::pair<const Automaton*, std::string>>& stateNames, const FSMTransition& transition, const Automaton* start) : transition(transition), current(start)
{
    states.reserve(stateNames.size());
    for(const auto& pair : stateNames)
    {
        states.push_back(pair.first);
        converter[pair.first] = pair.second;
    }
}

FSM::~FSM()
{
    for(const auto& state: states) delete state;
}

void FSM::operator()(const std::string& word) const
{
    current = transition((*current)(word), current);
}

const std::string& FSM::getState() const
{
    return converter.at(current);
}