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

    POSITION(int pos, char state, char next);
};

class AMOUNT : public Statement {
    int amount;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;

    AMOUNT(int amount, char state, char next);
};

class RANGEAND : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;

    RANGEAND(int min, int max, char state, char next);
};

class RANGEOR : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;

    RANGEOR(int min, int max, char state, char next);
};

class RANGEAMOUNT : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA() override;

    RANGEAMOUNT(int min, int max, char state, char next);
};

class AND : public Statement {
    Statement *first;
    Statement *second;
public:
    TempDFA generateTempDFA() override;

    AND(Statement *first, Statement *second);
};

class OR : public Statement {
    Statement *first;
    Statement *second;
public:
    TempDFA generateTempDFA() override;

    OR(Statement *first, Statement *second);
};

class NOT : public Statement {
    Statement *statement;
public:
    TempDFA generateTempDFA() override;

    NOT(Statement *statement);
};


#endif //GOL_STATEMENT_H
