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
#include "../automata/dfaPlusPlus.h"

enum Mult {
    cross, prioritizedUnion
};

struct TempDFA {
    std::vector<StatePlusPlus *> states;

    DFAPlusPlusTransition transition;

    StatePlusPlus *start;

    void print(const std::string &fileName, const StateMap &stateMap) const;

    void forAll(const StatePlusPlus *from, const StatePlusPlus *to);

    void forAllExcept(const StatePlusPlus *from, const StatePlusPlus *to, char except, const StatePlusPlus *toExcept);

    TempDFA multiply(const TempDFA &dfa, char def, Mult type);

    StatePlusPlus *addStatesRec(const StatePlusPlus *first, const StatePlusPlus *second,
                                std::map<std::pair<const StatePlusPlus *, const StatePlusPlus *>, const StatePlusPlus *> &map,
                                TempDFA &comb,
                                const TempDFA &dfa, char def, Mult multType);

    void TFAPlusPlus();

    StatePlusPlus *upgradeToMin(DFAPlusPlusTransition &minTransition, std::vector<StatePlusPlus *> &minStates,
                                std::map<std::set<const StatePlusPlus *>, StatePlusPlus *> &minStatesMap,
                                const std::set<const StatePlusPlus *> &minState);
};

class Statement {
public:
    virtual TempDFA generateTempDFA(char def) = 0;

    virtual ~Statement() = 0;
};

class POSITION : public Statement {
    int pos;
    char state;
    char next;
public:
    TempDFA generateTempDFA(char def) override;

    POSITION(int pos, char state, char next);
};

class AMOUNT : public Statement {
    int amount;
    char state;
    char next;
public:
    TempDFA generateTempDFA(char def) override;

    AMOUNT(int amount, char state, char next);
};

class RANGEAND : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA(char def) override;

    RANGEAND(int min, int max, char state, char next);
};

class RANGEOR : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA(char def) override;

    RANGEOR(int min, int max, char state, char next);
};

class RANGEAMOUNT : public Statement {
    int min;
    int max;
    char state;
    char next;
public:
    TempDFA generateTempDFA(char def) override;

    RANGEAMOUNT(int min, int max, char state, char next);
};

class AND : public Statement {
    Statement *first;
    Statement *second;
public:
    TempDFA generateTempDFA(char def) override;

    virtual ~AND();

    AND(Statement *first, Statement *second);
};

class OR : public Statement {
    Statement *first;
    Statement *second;
public:
    TempDFA generateTempDFA(char def) override;

    virtual ~OR();

    OR(Statement *first, Statement *second);
};

class NOT : public Statement {
    Statement *statement;
    char next;
public:
    TempDFA generateTempDFA(char def) override;

    virtual ~NOT();

    NOT(Statement *statement, char next);
};


#endif //GOL_STATEMENT_H
