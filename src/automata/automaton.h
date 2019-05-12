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
#include <stack>

struct Automaton
{
    enum Type{dfa, pda, tm, pa};

    virtual ~Automaton() = default;
    virtual bool operator()(const std::string& word, std::stack<char>& stack) const = 0;
    virtual Type getType() const = 0;
};
