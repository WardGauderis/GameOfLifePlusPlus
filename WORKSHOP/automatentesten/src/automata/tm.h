//============================================================================
// @name        : tm.h
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include "automaton.h"
#include "states.h"
#include "transitions.h"

#include <vector>


class TM : public Automaton
{
public:
    TM(const std::vector<char>& alphabet, const std::vector<char>& tapeAlphabet, const std::vector<TMState*>& states, const TMTransition& transition);
    bool operator()(const std::string& word) const override;

private:
    std::vector<char> alphabet;
    std::vector<char> tapeAlphabet;

    std::vector<TMState*> states;
    TMTransition transition;
    const TMState* start;
};


