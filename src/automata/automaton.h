//============================================================================
// @name        : Automaton.h
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include <string>

class Automaton
{
    virtual bool operator()(const std::string& word) const = 0;
};
