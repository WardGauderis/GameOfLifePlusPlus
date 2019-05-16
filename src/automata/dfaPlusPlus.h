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
#include <set>

class DFAPlusPlus : public Cell {
public:
    static std::vector<char> alphabet;

    static std::vector<StatePlusPlus*> states;

    static DFAPlusPlusTransition transition;

    void operator()(const std::string &word) const override;

    char getCurrent() const override;

    void TFAPlusPlus();

    DFAPlusPlus(const StatePlusPlus *current);

    void print(const std::string &fileName) const;

    DFAPlusPlus(const std::string &fileName);

private:
    mutable const StatePlusPlus *current;

    static StatePlusPlus *upgradeToMin(DFAPlusPlusTransition &minTransition, std::vector<StatePlusPlus *> &minStates,
                                std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> &minStatesMap,
                                const std::set<const StatePlusPlus *> &minState);

};


#endif //GOL_DFAPLUSPLUS_H
