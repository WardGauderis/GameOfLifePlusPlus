//============================================================================
// @name        : Statement.cpp
// @author      : Ward Gauderis
// @date        : 5/16/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include "statement.h"

POSITION::POSITION(int pos, char state, char next) : pos(pos), state(state), next(next) {}

AMOUNT::AMOUNT(int amount, char state, char next) : amount(amount), state(state), next(next) {}

RANGEAND::RANGEAND(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

RANGEOR::RANGEOR(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

RANGEAMOUNT::RANGEAMOUNT(int min, int max, char state, char next) : min(min), max(max), state(state), next(next) {}

AND::AND(Statement *first, Statement *second) : first(first), second(second) {}

OR::OR(Statement *first, Statement *second) : first(first), second(second) {}

NOT::NOT(Statement *statement) : statement(statement) {}