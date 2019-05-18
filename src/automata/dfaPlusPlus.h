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

struct StateMap : public std::vector<std::tuple<std::string, char, Color, char>>{
    Color color(char c) const;

    char character(const std::string &name) const;

    char next(char c) const;
};


class DFAPlusPlus : public Cell {
public:
    static std::vector<char> alphabet;

    static std::vector<StatePlusPlus*> states;

    static DFAPlusPlusTransition transition;

    static StatePlusPlus *start;

    void operator()(const std::string &word) const override;

    char getCurrent() const override;

    static void TFAPlusPlus();

    static void print(const std::string &fileName, const StateMap &stateMap);

    DFAPlusPlus(const std::string &fileName);

    DFAPlusPlus(char current);

private:
    mutable char current;

    static StatePlusPlus *upgradeToMin(DFAPlusPlusTransition &minTransition, std::vector<StatePlusPlus *> &minStates,
                                       std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> &minStatesMap,
                                       const std::set<const StatePlusPlus *> &minState);

};


#endif //GOL_DFAPLUSPLUS_H
