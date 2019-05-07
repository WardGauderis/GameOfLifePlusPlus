//============================================================================
// @name        : transitions.cpp
// @author      : Thomas Dooms
// @date        : 5/6/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "transitions.h"

const State* DFATransition::empty = nullptr;
const std::vector<const State*> NFATransition::empty = {};
const std::tuple<const PDAState*, char, char> PDATransition::empty = {};
const std::tuple<const TMState*, char, char> TMTransition::empty = {};
const Automaton* FSMTransition::empty = nullptr;
const std::vector<std::pair<double, const State*>> PATransition::empty = {};
const StatePlusPlus* DFAPlusPlusTransition::empty = nullptr;