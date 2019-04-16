//============================================================================
// @name        : dfaPlusPlus.h
// @author      : Ward Gauderis
// @date        : 4/16/19
// @version     : 1.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Subclass of Cell with DFA++
//============================================================================
#ifndef GOL_DFAPLUSPLUS_H
#define GOL_DFAPLUSPLUS_H

#include "cell.h"
#include "states.h"
#include "transitions.h"
#include <vector>

class dfaPlusPlus : public Cell {
public:
    dfaPlusPlus(const std::vector<const StatePlusPlus *const> &states, DFAPlusPlusTransition transition,
                const StatePlusPlus *current);

    void operator()(const std::string &word) const override;

    const std::string &getState() const override;

    ~dfaPlusPlus() override;

private:
    const std::vector<const StatePlusPlus *const> states;
    const DFAPlusPlusTransition transition;
    mutable const StatePlusPlus *current;

};


#endif //GOL_DFAPLUSPLUS_H
