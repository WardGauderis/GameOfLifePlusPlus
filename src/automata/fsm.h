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
#include <stack>

class FSM : public Cell
{
public:
    FSM(const Automaton* start);

    static void init(const std::map<const Automaton*, std::pair<char, bool>>& stateData, const FSMTransition& transition);

    void operator()(const std::string &word) const override;
    char getCurrent() const override;

private:
    static std::map<const Automaton*, std::pair<char, bool>> states;
    static FSMTransition transition;

    mutable const Automaton* current;
    mutable std::stack<char> stack;
};


#endif //GOL_FSM_H
