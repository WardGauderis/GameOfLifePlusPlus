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

class FSM: public Cell {
public:
    FSM(const std::vector<std::pair<const Automaton *, std::string>> &stateNames, const FSMTransition &transition,
        const Automaton *start);

    ~FSM();

    void operator()(const std::string &word) const override ;

    const std::string &getState() const override;

private:
    std::vector<const Automaton *> states;
    FSMTransition transition;
    mutable const Automaton *current;

    std::map<const Automaton *, std::string> converter;
};


#endif //GOL_FSM_H
