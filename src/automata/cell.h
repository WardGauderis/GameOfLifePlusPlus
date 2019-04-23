//============================================================================
// @name        : Cell.h
// @author      : Ward Gauderis
// @date        : 4/16/19
// @version     : 1.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Pure virtual class for a cell in the cellular automaton.
//============================================================================
#ifndef GOL_CELL_H
#define GOL_CELL_H

#include <string>

class Cell {
public:
    virtual void operator()(const std::string &word) const = 0;

    virtual char getState() const = 0;
};


#endif //GOL_CELL_H
