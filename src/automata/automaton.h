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
#include <vector>

struct Automaton
{
    virtual ~Automaton() = default;
    virtual bool operator()(const std::string& word) const = 0;
    static std::vector<char> alphabet; // moet eerst geïnitialiseerd worden, daarna pas aangepast
};
