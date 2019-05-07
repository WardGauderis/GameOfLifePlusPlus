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
#include "../gui/color.h"

typedef std::tuple<char, std::string, Color> Data;

struct Cell
{
    virtual ~Cell() = default;
    virtual void operator()(const std::string &word) const = 0;
    virtual char getCurrent() const = 0;
};


#endif //GOL_CELL_H
