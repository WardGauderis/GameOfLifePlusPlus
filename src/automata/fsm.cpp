//============================================================================
// @name        : fsm.cpp
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "fsm.h"


void FSM::operator()(const std::string& word) const
{
    current = transition((*current)(word), current);
}

const std::string& FSM::getState() const
{
    return converter.at(current);
}