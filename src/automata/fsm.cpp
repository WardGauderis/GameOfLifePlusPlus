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

std::map<const Automaton*, Data> FSM::states = {};
FSMTransition FSM::transition = {};

FSM::FSM(const Automaton* start) : current(start) {}

void FSM::init(const std::vector<std::tuple<const Automaton*, const std::string&, const Color&>>& stateData, const FSMTransition& transition)
{
    char index = 65;
    for(const auto& state : stateData)
    {
        FSM::states[std::get<0>(state)] = {index, std::get<1>(state), std::get<2>(state)};
        index++;
    }
    FSM::transition = transition;
}

void FSM::operator()(const std::string& word) const
{
    current = transition((*current)(word), current);
}

const Data& FSM::getData() const
{
    return states.at(current);
}