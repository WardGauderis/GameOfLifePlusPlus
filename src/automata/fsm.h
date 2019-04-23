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

#include <vector>
#include <map>
#include "transitions.h"
#include "automaton.h"
#include "cell.h"

class FSM: public Cell
{
public:
    FSM(const Automaton* start);
    ~FSM();

    static void init(const std::vector<const Automaton*>& states, const FSMTransition& transition);

    void operator()(const std::string &word) const override;

    char getState() const override;

private:
    static std::vector<const Automaton*> states;
    static FSMTransition transition;

    mutable const Automaton* current;
};


#endif //GOL_FSM_H
