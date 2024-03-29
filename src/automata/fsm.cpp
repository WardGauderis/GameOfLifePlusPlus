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

std::map<const Automaton*, std::pair<char, bool>> FSM::states = {};
FSMTransition FSM::transition = {};
std::map<char, const Automaton*> FSM::charToState;

FSM::FSM(char start) : current(charToState.at(start)), stack() {}

void FSM::init(const std::map<const Automaton*, std::pair<char, bool>>& states, const FSMTransition& transition, const std::map<char, const Automaton*>& charToState)
{
    FSM::states = states;
    FSM::transition = transition;
    FSM::charToState = charToState;
}

void FSM::operator()(const std::string& word) const
{
    do{ current = transition[{(*current)(word, stack), current}]; } while (states.at(current).second);
}

char FSM::getCurrent() const
{
    return states.at(current).first;
}