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
    dfaPlusPlus(std::vector<StatePlusPlus *> states, DFAPlusPlusTransition transition,
                const StatePlusPlus *current, std::vector<char> alphabet);

    void operator()(const std::string &word) const override;

    const std::string &getState() const override;

    ~dfaPlusPlus();

    void TFAPlusPlus();

private:
    const std::vector<StatePlusPlus *> states;
    const DFAPlusPlusTransition transition;
    mutable const StatePlusPlus *current;
    const std::vector<char> alphabet;

};


#endif //GOL_DFAPLUSPLUS_H
