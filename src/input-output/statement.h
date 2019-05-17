//============================================================================
// @name        : Statement.h
// @author      : Ward Gauderis
// @date        : 5/16/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef GOL_STATEMENT_H
#define GOL_STATEMENT_H

#include "../automata/states.h"
#include "../automata/transitions.h"

struct TempDFA {
    std::vector<StatePlusPlus *> states;

    DFAPlusPlusTransition transition;

    StatePlusPlus *start;

};

class Statement {
public:
    virtual TempDFA generateTempDFA() = 0;

};

class POSITION : public Statement {
    int pos;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;
};

class AMOUNT : public Statement {
    int amount;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;
};

class RANGEAND : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;
};

class RANGEOR : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;
};

class RANGEAMOUNT : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;
};

class AND : public Statement {
    Statement *first;
    Statement *second;
public:
    TempDFA generateTempDFA() override;
};

class OR : public Statement {
    Statement *first;
    Statement *second;
public:
    TempDFA generateTempDFA() override;
};

class NOT : public Statement {
    Statement *statement;
public:
    TempDFA generateTempDFA() override;
};

class DEFAULT : public Statement {
    char next;
public:
    TempDFA generateTempDFA() override;
};


#endif //GOL_STATEMENT_H
