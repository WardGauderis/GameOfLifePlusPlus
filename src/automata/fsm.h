//============================================================================
// @name        : fsm.h
// @author      : Thomas Dooms
// @date        : 4/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef GOL_FSM_H
#define GOL_FSM_H

#include <string>
#include <vector>
#include <map>
#include "transitions.h"
#include "automaton.h"

class FSM
{
public:
    FSM(const std::vector<std::pair<const Automaton*, std::string>>& stateNames, const FSMTransition& transition, const Automaton* start);
    ~FSM();

    void operator()(const std::string& word) const;
    const std::string& getState() const;

private:
    std::vector<const Automaton*> states;
    FSMTransition transition;
    mutable const Automaton* current;

    std::map<const Automaton* , std::string> converter;
};


#endif //GOL_FSM_H
